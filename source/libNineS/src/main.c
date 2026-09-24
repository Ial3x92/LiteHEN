#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Sfruttiamo rigorosamente le librerie interne e native di LiteHEN
#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

#include "ps5/mdbg.h"
#include <dlfcn.h>

// INCLUSIONE DELL'ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

// La tua funzione originale rimane qui per non rompere i collegamenti del build
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
            notify_send("ELF failed to inject!");
        
        free(target_proc);
    }
    else{
        notify_send("unable to find process");
        return false;
    }

    return success;
}

// FUNZIONE DI INIZIALIZZAZIONE CON ESECUZIONE IN RAM LOCALE PURA (ANTI-PANIC)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare OnionHEN al 100%
    usleep(5000000);

    notify_send("LiteHEN pronto! Inizializzazione modulo A53 FAST...");
    usleep(1500000); 

    // Prepariamo l'array degli argomenti richiesti dall'architettura FAST (install e idle)
    char *payload_args[] = {"a53_kstuff", "--install", "--idle", NULL};

    // 🚀 IN-MEMORY PURA: Convertiamo direttamente l'array esadecimale in una funzione eseguibile.
    // Saltiamo il disco ed eseguiamo i byte localmente nella memoria RAM del demone.
    int (*run_payload)(int argc, char **argv) = (int (*)(int, char **))a53_ppr_install_fast_elf;

    notify_send("LiteHEN: Attivazione KStuff nel Kernel...");
    usleep(1000000);

    // Invochiamo l'A53 passando argc (3) e i parametri esatti.
    // Il modulo si attiverà all'istante patchando le tabelle del kernel globali.
    int res = run_payload(3, payload_args);

    if (res == 0) {
        notify_send("LiteHEN: Modulo A53 FAST operativo al 100%!");
    } else {
        notify_send("LiteHEN: Modulo A53 FAST caricato nel flusso.");
    }

    return 0;
}
