#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Incluso esplicitamente per supportare il tipo bool in C standard

// Utilizziamo SOLO le librerie interne stabili di LiteHEN/OnionHEN
#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

#include "ps5/mdbg.h"
#include <dlfcn.h>

// INCLUSIONE DELL'ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

// Funzione originale di OnionHEN per l'iniezione tramite libhijacker
bool Inject_Toolbox(int pid, uint8_t *elf)
{                                  
    if (pid < 0 || !elf) {
        notify_send("Invalid ToolBox arguments");
        return false;
    } 
    
    bool success = false;
    struct proc* target_proc = get_proc_by_pid(pid);
    
    if (target_proc) {
        // Chiama l'injector nativo (ptrace + stager + set_ucred_to_ptrace)
        if (inject_elf(target_proc, elf)) {
            success = true;
        }
        // Libera la memoria della struttura processo allocata dal kernel/framework
        free(target_proc);
    } else {
        return false;
    }

    return success;
}

// FUNZIONE DI INIZIALIZZAZIONE NATIVA (Richiamata asincronamente da main.cpp)
// Riceve il flag is_fw_800 dal C++ per sincronizzare perfettamente lo stack del thread
int init_nineS(bool is_fw_800)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare OnionHEN al 100% in background
    usleep(5000000);

    // Controllo di sicurezza preventivo per evitare Signal 11 se l'header non è mappato bene
    if (!a53_ppr_install_fast_elf) {
        notify_send("Errore OnionHEN: Array A53 FAST non trovato in memoria!");
        return -1;
    }

    notify_send("OnionHEN: Iniezione modulo A53 FAST in corso...");
    usleep(1500000); 

    bool iniettato = false;
    
    // Lista dei PID di sistema standard su PS5 dove risiede stabilmente SceShellUI
    int target_pids[] = {81, 82, 80, 83, 84, 85};
    int num_pids = sizeof(target_pids) / sizeof(int);

    // 🚀 LOGICA NATIVA: Cicliamo sui PID grafici esterni usando Inject_Toolbox (libhijacker)
    for (int i = 0; i < num_pids; i++) {
        if (Inject_Toolbox(target_pids[i], (uint8_t *)a53_ppr_install_fast_elf)) {
            iniettato = true;
            break; // Successo! Usciamo immediatamente dal ciclo per evitare iniezioni multiple
        }
        // Micro-pausa di respiro (200ms) tra un tentativo e l'altro per la stabilità del kernel
        usleep(200000); 
    }

    if (iniettato) {
        notify_send("OnionHEN: Modulo A53 FAST attivato in ShellUI!");
    } else {
        if (is_fw_800) {
            notify_send("Errore: Fallito aggancio A53. Rilevato FW >= 8.00");
        } else {
            notify_send("Errore: Impossibile agganciare A53 alla Toolbox.");
        }
    }

    return 0;
}
