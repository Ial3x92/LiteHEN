/* Copyright (C) 2026 OnionHEN / LightningMods */

#include "launch_pipeline.h"

#include "bootstrap_notify.h"

// MODIFICA: Rimosso include di kstuff_probe.h

#include <elfldr_remote.h>
#include <onion/log.h>
#include <onion/payload.h>
#include <onion/platform.h>
#include <onion/proc_query.h>
#include <onion/ready.h>

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern uint8_t daemon_start[];
extern const unsigned int daemon_size;
extern uint8_t util_start[];
extern const unsigned int util_size;
extern uint8_t onion_elfldr_start[];
extern const unsigned int onion_elfldr_size;

// MODIFICA: Rimossi i puntatori esterni a kstuff_start e kstuff_size

int sceKernelGetProcessName(int pid, char *name);
int sceKernelMprotect(void *addr, size_t len, int prot);

typedef struct LaunchContext {
  uint16_t loader_port;
} LaunchContext;

static void stop_owned_service(const char *ready_name,
                               const char *expected_process_name) {
  pid_t pid = -1;
  if (!onion_ready_read_pid(ready_name, &pid) || pid <= 1) {
    onion_ready_clear(ready_name);
    return;
  }

  char process_name[32] = {0};
  if (!onion_proc_is_alive(pid) ||
      sceKernelGetProcessName(pid, process_name) != 0 ||
      strcmp(process_name, expected_process_name) != 0) {
    LOG_WARN("Ignoring stale %s ownership marker pid=%d", ready_name,
             (int)pid);
    onion_ready_clear(ready_name);
    return;
  }

  LOG_INFO("Stopping owned %s pid=%d", ready_name, (int)pid);
  if (kill(pid, SIGKILL) != 0 && errno != ESRCH)
    LOG_ERROR("Failed to stop owned %s pid=%d: %s", ready_name, (int)pid,
              strerror(errno));
  onion_ready_clear(ready_name);
}

static bool loader_available(uint16_t port) {
  return port == ONION_ELFLDR_PORT ? elfldr_remote_onion_available()
                                   : elfldr_remote_available_on(port);
}

static bool wait_onion_loader(int timeout_ms) {
  const int poll_ms = 200;
  for (int waited = 0; waited < timeout_ms; waited += poll_ms) {
    if (elfldr_remote_onion_available())
      return true;
    usleep((useconds_t)poll_ms * 1000);
  }
  return false;
}

static bool ensure_private_loader(void) {
  if (elfldr_remote_onion_available()) {
    LOG_WARN("embedded elfldr :9020 already available");
    return true;
  }

  LOG_DEBUG("Starting embedded elfldr on %u via external %u ...",
            ONION_ELFLDR_PORT, ELFLDR_REMOTE_PORT);
  if (!elfldr_remote_send_bytes_to(ELFLDR_REMOTE_PORT, onion_elfldr_start,
                                   onion_elfldr_size)) {
    LOG_ERROR("  embedded elfldr launch send failed");
    return false;
  }

  if (wait_onion_loader(10000)) {
    LOG_DEBUG("  embedded elfldr :9020 ready");
    return true;
  }

  LOG_DEBUG("  embedded elfldr :9020 did not become ready");
  return false;
}

static bool launch_blob(LaunchContext *context, const uint8_t *elf, size_t size,
                        const char *label, const char *wait_name) {
  if (context->loader_port != ELFLDR_REMOTE_PORT &&
      !loader_available(context->loader_port)) {
    context->loader_port = ELFLDR_REMOTE_PORT;
  }

  LOG_DEBUG("%u memory ELF: %s (%zu bytes)", context->loader_port, label,
            size);
  if (!elfldr_remote_send_bytes_to(context->loader_port, elf, size)) {
    if (context->loader_port != ELFLDR_REMOTE_PORT &&
        elfldr_remote_send_bytes_to(ELFLDR_REMOTE_PORT, elf, size)) {
      LOG_ERROR("  send via %u failed; fallback %u accepted %s",
                context->loader_port, ELFLDR_REMOTE_PORT, label);
      context->loader_port = ELFLDR_REMOTE_PORT;
    } else {
      LOG_ERROR("  send FAILED %s", label);
      return false;
    }
  }

  for (int i = 0; i < 30; ++i) {
    if (wait_name && onion_find_pid_substr(wait_name) > 0) {
      LOG_DEBUG("  running: %s", wait_name);
      return true;
    }
    sleep(1);
  }
  LOG_DEBUG("  sent %s (process name not seen yet, continuing)", label);
  return true;
}

static void prepare_service_markers(void) {
  stop_owned_service(ONION_READY_DAEMON, "onion_daemon.elf");
  stop_owned_service(ONION_READY_UTIL, "onion_util.elf");
  onion_ready_clear(ONION_FLAG_UTIL_BOOTED);
  onion_ready_clear(ONION_FLAG_FPS_OVERLAY);
}

static bool launch_util(LaunchContext *context) {
  LOG_DEBUG("Starting util via %u ...", context->loader_port);
  if (!launch_blob(context, util_start, util_size, "util", "onion_util.elf")) {
    bootstrap_notify("notify.elfldr.launch_util");
    return false;
  }
  if (!onion_ready_wait(ONION_READY_UTIL, 15000, 200))
    LOG_WARN("util ready timeout -- continuing (process may still be starting)");
  return true;
}

// MODIFICA: Funzione stubbata per saltare interamente il caricamento di kstuff
static void launch_kstuff(LaunchContext *context, uint32_t firmware_version,
                          bool autoload) {
  (void)context;
  (void)firmware_version;
  (void)autoload;
  LOG_DEBUG("kstuff bypass option active - skipping kernel payload loading");
  onion_ready_signal(ONION_READY_KSTUFF);
}

static bool launch_daemon(LaunchContext *context) {
  LOG_DEBUG("Starting daemon via %u (toolbox inject) ...",
            context->loader_port);
  if (!launch_blob(context, daemon_start, daemon_size, "daemon",
                   "onion_daemon.elf")) {
    bootstrap_notify("notify.elfldr.launch_daemon");
    return false;
  }
  if (!onion_ready_wait(ONION_READY_DAEMON, 20000, 200))
    LOG_WARN("daemon ready timeout -- continuing");
  return true;
}

int bootstrap_launch_services(uint32_t firmware_version, bool kstuff_autoload) {
  if (!elfldr_remote_available()) {
    LOG_DEBUG("FATAL: no elfldr on 127.0.0.1:9021");
    bootstrap_notify("notify.elfldr.need_9021");
    return -2;
  }

  LaunchContext context = {
      .loader_port = ensure_private_loader() ? ONION_ELFLDR_PORT
                                             : ELFLDR_REMOTE_PORT,
  };
  LOG_DEBUG("payload loader port: %u", context.loader_port);
  LOG_DEBUG("launching embedded ELFs (serialized)");
  sleep(3);

  prepare_service_markers();
  if (!launch_util(&context))
    return -2;
  launch_kstuff(&context, firmware_version, kstuff_autoload);
  return launch_daemon(&context) ? 0 : -2;
}
