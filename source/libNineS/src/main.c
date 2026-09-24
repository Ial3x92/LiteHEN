#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

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

// La tua funzione originale e intatta di OnionHEN per l'iniezione tramite libhijacker
bool Inject_Toolbox(int pid, uint8_t * elf)
{                                  
    if(pid < 0 || !elf){
        notify_send("Invalid ToolBox arguments");
        return false;
    } 
    bool success = true;
    struct proc* target_proc = get_proc_by_pid(pid);
    if (target_proc)
    {
        // Chiama l'injector nativo (ptrace + stager + set_ucred_to_ptrace)
        if (!(success = inject_elf(target_proc, elf))) {
            // Silenziamo per evitare loop di notifiche nel ciclo
            success = false;
        }
        free(target_proc);
    }
    else{
        return false;
    }

    return success;
}

// FUNZIONE DI INIZIALIZZAZIONE NATIVA (Richiamata asincronamente da main.cpp)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare OnionHEN al 100%
    usleep(5000000);

    notify_send("OnionHEN: Iniezione modulo A53 FAST in corso...");
    usleep(1500000); 

    bool iniettato = false;
    
    // Lista dei PID di sistema standard su PS5 dove risiede stabilmente SceShellUI
    int target_pids[] = {81, 82, 80, 83, 84, 85};
    int num_pids = sizeof(target_pids) / sizeof(target_pids[0]);

    // 🚀 LOGICA NATIVA: Cicliamo sui PID grafici esterni usando la tua Inject_Toolbox (libhijacker)
    for (int i = 0; i < num_pids; i++) {
        if (Inject_Toolbox(target_pids[i], (uint8_t *)a53_ppr_install_fast_elf)) {
            iniettato = true;
            break; // Successo! Usciamo immediatamente dal ciclo
        }
        usleep(200000); // Micro-pausa di respiro tra un tentativo e l'altro
    }

    if (iniettato) {
        notify_send("OnionHEN: Modulo A53 FAST attivato in ShellUI!");
    } else {
        notify_send("Errore: Impossibile agganciare A53 alla Toolbox.");
    }

    return 0;
}
