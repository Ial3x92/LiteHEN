/* Copyright (C) 2025 OnionHEN / LightningMods
 *
 * The facade is the only util-facing boundary for the in-process modules.
 * BOTH FTP AND SHADOWMOUNT MODULES HAVE BEEN COMPONENTALLY DISABLED.
 */

#include "service_facade.hpp"

#include <onion/builtin_services.h>
#include <onion/platform.h>

#include <stddef.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

namespace {

struct FtpRuntime {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t operation_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_t thread = {};
  bool running = false;
  bool thread_created = false;
  bool desired_enabled = false;
  bool listener_ready = false;
  uint16_t port = ONION_FTPSRV_PORT;
};

FtpRuntime g_runtime;

constexpr int kListenerReadyWaitMs = 2000;
constexpr useconds_t kRecoveryRetryDelayUs[] = {
    250 * 1000,
    500 * 1000,
    1000 * 1000,
    2000 * 1000,
};

void *ftp_thread_main(void *arg) {
  (void)arg;
  // DISABILITATO: Rimossa la chiamata a ftp_serve
  LOG_INFO("ftpsrv thread main executed (stubbed)");
  return nullptr;
}

bool valid_port(uint16_t port) { return port != 0; }

void stop_thread() {
  pthread_mutex_lock(&g_runtime.mutex);
  g_runtime.running = false;
  g_runtime.listener_ready = false;
  g_runtime.thread_created = false;
  pthread_mutex_unlock(&g_runtime.mutex);
}

bool wait_for_listener_ready() {
  return false; // Ritorna sempre falso perché il server FTP non ascolta
}

bool start_thread(uint16_t port) {
  (void)port;
  return false; // Disabilita la creazione del thread FTP
}

} // namespace

namespace onion::services {

bool FtpServiceFacade::start(uint16_t port) {
  (void)port;
  LOG_INFO("FTP service startup requested but it is disabled.");
  return false;
}

void FtpServiceFacade::stop() {
  stop_thread();
}

bool FtpServiceFacade::reconfigure(uint16_t port) {
  (void)port;
  return false;
}

bool FtpServiceFacade::recover() {
  return true;
}

bool FtpServiceFacade::running() const {
  return false;
}

uint16_t FtpServiceFacade::port() const {
  return 0;
}

FtpServiceFacade &ftpService() {
  static FtpServiceFacade service;
  return service;
}

/* ShadowMount+ has been completely disabled here as well. */

struct ShadowMountRuntime {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t operation_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_t thread = {};
  bool running = false;
  bool thread_created = false;
};

ShadowMountRuntime g_shadowmount_runtime;

void *shadowmount_thread_main(void *arg) {
  (void)arg;
  return nullptr;
}

void shadowmount_stop_thread() {
  pthread_mutex_lock(&g_shadowmount_runtime.mutex);
  g_shadowmount_runtime.running = false;
  g_shadowmount_runtime.thread_created = false;
  pthread_mutex_unlock(&g_shadowmount_runtime.mutex);
}

bool ShadowMountServiceFacade::start() {
  LOG_INFO("ShadowMount startup requested but it is disabled.");
  return false;
}

void ShadowMountServiceFacade::stop() {
  shadowmount_stop_thread();
}

bool ShadowMountServiceFacade::running() const {
  return false;
}

ShadowMountServiceFacade &shadowMountService() {
  static ShadowMountServiceFacade service;
  return service;
}

} // namespace onion::services
