/* Copyright (C) 2025 OnionHEN / LightningMods
 *
 * The facade is the only util-facing boundary for the in-process FTP module.
 * It owns the serving thread and keeps UI/IPC code independent from the
 * third-party implementation details.
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

// Le funzioni interne del thread FTP sono state rimosse poiché il servizio è disabilitato.
void stop_thread() {
  pthread_mutex_lock(&g_runtime.mutex);
  g_runtime.running = false;
  g_runtime.listener_ready = false;
  g_runtime.thread_created = false;
  pthread_mutex_unlock(&g_runtime.mutex);
}

} // namespace

namespace onion::services {

bool FtpServiceFacade::start(uint16_t port) {
  // Servizio disabilitato nella versione Lite
  LOG_INFO("FTP service is disabled in this build.");
  return false;
}

void FtpServiceFacade::stop() {
  pthread_mutex_lock(&g_runtime.operation_mutex);
  g_runtime.desired_enabled = false;
  stop_thread();
  pthread_mutex_unlock(&g_runtime.operation_mutex);
}

bool FtpServiceFacade::reconfigure(uint16_t port) {
  pthread_mutex_lock(&g_runtime.operation_mutex);
  pthread_mutex_lock(&g_runtime.mutex);
  g_runtime.port = port;
  pthread_mutex_unlock(&g_runtime.mutex);
  pthread_mutex_unlock(&g_runtime.operation_mutex);
  return true;
}

bool FtpServiceFacade::recover() {
  pthread_mutex_lock(&g_runtime.operation_mutex);
  pthread_mutex_lock(&g_runtime.mutex);
  const bool desired = g_runtime.desired_enabled;
  pthread_mutex_unlock(&g_runtime.mutex);

  if (!desired) {
    pthread_mutex_unlock(&g_runtime.operation_mutex);
    LOG_DEBUG("ftpsrv recovery skipped; service is disabled");
    return true;
  }

  stop_thread();
  pthread_mutex_unlock(&g_runtime.operation_mutex);
  return false;
}

bool FtpServiceFacade::running() const {
  pthread_mutex_lock(&g_runtime.mutex);
  const bool value = g_runtime.listener_ready;
  pthread_mutex_unlock(&g_runtime.mutex);
  return value;
}

uint16_t FtpServiceFacade::port() const {
  pthread_mutex_lock(&g_runtime.mutex);
  const uint16_t value = g_runtime.port;
  pthread_mutex_unlock(&g_runtime.mutex);
  return value;
}

FtpServiceFacade &ftpService() {
  static FtpServiceFacade service;
  return service;
}

/* ShadowMount+ runs the same way: one worker thread owned by the facade, with
 * third-party details confined to the adapter (shadowmount_main.cpp). */

struct ShadowMountRuntime {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t operation_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_t thread = {};
  bool running = false;
  bool thread_created = false;
};

ShadowMountRuntime g_shadowmount_runtime;

void shadowmount_stop_thread() {
  pthread_mutex_lock(&g_shadowmount_runtime.mutex);
  g_shadowmount_runtime.running = false;
  g_shadowmount_runtime.thread_created = false;
  pthread_mutex_unlock(&g_shadowmount_runtime.mutex);
}

bool ShadowMountServiceFacade::start() {
  pthread_mutex_lock(&g_shadowmount_runtime.operation_mutex);
  shadowmount_stop_thread();
  pthread_mutex_unlock(&g_shadowmount_runtime.operation_mutex);

  LOG_INFO("shadowmount module is disabled in this build");
  return false;
}

void ShadowMountServiceFacade::stop() {
  pthread_mutex_lock(&g_shadowmount_runtime.operation_mutex);
  shadowmount_stop_thread();
  pthread_mutex_unlock(&g_shadowmount_runtime.operation_mutex);
}

bool ShadowMountServiceFacade::running() const {
  pthread_mutex_lock(&g_shadowmount_runtime.mutex);
  const bool value = g_shadowmount_runtime.running;
  pthread_mutex_unlock(&g_shadowmount_runtime.mutex);
  return value;
}

ShadowMountServiceFacade &shadowMountService() {
  static ShadowMountServiceFacade service;
  return service;
}

} // namespace onion::services

