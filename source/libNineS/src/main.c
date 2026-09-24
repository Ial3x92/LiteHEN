#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Utilizziamo le librerie interne e native di LiteHEN
#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"

#include "ps5/mdbg.h"
#include <dlfcn.h>

// INCLUSIONE DELL'ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

// La tua funzione originale e intatta di LiteHEN per l'iniezione
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
        if (!(success = inject_elf(target_proc, elf)))
            success = false; // Silenziamo il notify interno durante i tentativi del ciclo
        
        free(target_proc);
    }
    else{
        return false;
    }

    return success;
}

// FUNZIONE DI INIZIALIZZAZIONE NATIVA LITEHEN
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Permette al demone principale di OnionHEN di fare il boot completo
    usleep(5000000);

    notify_send("LiteHEN: Caricamento modulo A53 FAST...");
    usleep(1500000);

    bool iniettato = false;
    
    // Lista dei PID di sistema standard su PS5 dove risiede stabilmente SceShellUI
    int target_pids[] = {81, 82, 80, 83, 84};
    int num_pids = sizeof(target_pids) / sizeof(target_pids[0]);

    // 🚀 LOGICA NATIVA LITEHEN: Cicliamo sui PID grafici esterni usando la tua Inject_Toolbox
    for (int i = 0; i < num_pids; i++) {
        if (Inject_Toolbox(target_pids[i], (uint8_t *)a53_ppr_install_fast_elf)) {
            iniettato = true;
            break; // Successo! Usciamo immediatamente dal ciclo
        }
        usleep(100000); // Micro-pausa di respiro tra un tentativo e l'altro
    }

    if (iniettato) {
        notify_send("LiteHEN: Modulo A53 FAST attivato in ShellUI!");
    } else {
        notify_send("Errore: Vettore ShellUI non trovato.");
    }

    return 0;
}
