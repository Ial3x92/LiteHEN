#pragma once

#include "cheats/sync/types.hpp" // Rimosso l'engine mancante per evitare errori di inclusione

#include <cstdint>
#include <mutex>
#include <string>

#ifdef __cplusplus
#include <onion/settings.hpp>
#endif

namespace onion::cheats::sync {

class IHttpTransport;

struct CheatSyncStatus {
  enum class State { Idle, Running, Ok, Error } state = State::Idle;
  uint32_t task_id = 0;
  CheatMirrorId mirror = CheatMirrorId::Github;
  std::string url;
  std::string catalog_id;
  std::string error;
  std::string phase;
  int progress_percent = -1;
  size_t completed = 0;
  size_t total = 0;
};

/**
 * Process facade for IPC. Owns the worker thread and last status.
 * MODIFICATO: Versione LITE con stub inlined per evitare errori di linking.
 */
class CheatSyncService {
public:
  enum class StartResult { Started, AlreadyRunning, Rejected };

  // Ritorna un'istanza fittizia statica locale
  static CheatSyncService &instance() {
    static CheatSyncService inst;
    return inst;
  }

  // Rifiuta immediatamente qualsiasi richiesta di download online
  StartResult start(const onion::Settings &settings, const char *catalog_id,
                    const char *mirror_override, uint32_t *task_id = nullptr) {
    if (task_id) *task_id = 0;
    return StartResult::Rejected;
  }

  // Ritorna falso poiché non ci sono task attivi da annullare
  bool cancel(uint32_t task_id) { return false; }
  bool cancellationRequested() const { return false; }

  // Restituisce sempre uno stato Idle vuoto
  CheatSyncStatus status() const { return CheatSyncStatus{}; }

  void setHttpTransportForTest(IHttpTransport *http) {}

  void worker(onion::Settings settings, std::string catalog_id,
              std::string mirror_override, uint32_t task_id) {}

  void noteProgress(const char *phase, int percent, size_t completed,
                    size_t total) {}

  CheatSyncService(const CheatSyncService &) = delete;
  CheatSyncService &operator=(const CheatSyncService &) = delete;

private:
  // Costruttore e distruttore vuoti inlined per non richiedere il file .cpp
  CheatSyncService() : running_(false), cancel_requested_(false), next_task_id_(0), test_http_(nullptr) {}
  ~CheatSyncService() {}

  mutable std::mutex mu_;
  CheatSyncStatus status_{};
  bool running_ = false;
  bool cancel_requested_ = false;
  uint32_t next_task_id_ = 0;
  IHttpTransport *test_http_ = nullptr;
};

} // namespace onion::cheats::sync
