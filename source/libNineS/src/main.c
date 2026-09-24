#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Includiamo l'header del mini-loader nativo presente nel tuo repository
#include "onion/elfldr.h"

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

// FUNZIONE DI INIZIALIZZAZIONE CON ELEVAZIONE DI SICUREZZA NATIVA LITEHEN
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare OnionHEN al 100%
    usleep(5000000);

    notify_send("LiteHEN pronto! Configurazione sicurezza RAM...");
    usleep(1500000); 

    // Backup dell'AuthID corrente (Logica nativa copiata dall'injector)
    uint64_t original_authid = kernel_get_ucred_authid(getpid());
    
    // 🚀 ELEVAZIONE DI SICUREZZA DI LITEHEN 🚀
    // Cambiamo l'AuthID per sbloccare i permessi di mappatura ptrace
    set_ucred_to_ptrace();

    notify_send("LiteHEN: Avvio modulo A53 FAST in RAM...");
    
    // Ora che siamo autorizzati da set_ucred_to_ptrace(), eseguiamo elfldr_load
    // sul nostro processo locale (getpid()). Verrà accettato istantaneamente.
    intptr_t res = elfldr_load(getpid(), (uint8_t *)a53_ppr_install_fast_elf);

    // 🚀 RIPRISTINO DI SICUREZZA DI LITEHEN 🚀
    // Rimettiamo l'AuthID originale per blindare nuovamente il Kernel ed evitare Kernel Panic
    if (original_authid != 0) {
        kernel_set_ucred_authid(getpid(), original_authid);
    }

    // Verdetto finale a schermo
    if (res >= 0) {
        notify_send("LiteHEN: Modulo A53 FAST attivato stabilmente!");
    } else {
        notify_send("Errore: Il Kernel ha rifiutato la mappatura.");
    }

    return 0;
}
