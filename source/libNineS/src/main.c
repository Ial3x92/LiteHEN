#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Usiamo SOLO le librerie interne stabili di LiteHEN
#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

#include "ps5/mdbg.h"
#include <dlfcn.h>

// INCLUSIONE DELL'ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

// La tua funzione originale e intatta di LiteHEN
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

// INTERRUTTORE DI INIZIALIZZAZIONE NATIVO LITEHEN
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare OnionHEN al 100%
    usleep(5000000);

    notify_send("LiteHEN: Sincronizzazione Toolbox con modulo FAST...");
    usleep(1500000);

    // 🚀 INTEGRAZIONE NATIVA NELLA TOOLBOX 🚀
    // Passiamo 0 alla tua Inject_Toolbox originale. Questo aggancia l'A53 FAST
    // al gestore dei payload di LiteHEN. Il demone principale lo avvierà
    // passandogli i parametri giusti ed evitando che rimanga congelato in RAM.
    if (Inject_Toolbox(0, (uint8_t *)a53_ppr_install_fast_elf)) {
        notify_send("LiteHEN: Modulo A53 FAST integrato nella Toolbox!");
    } else {
        notify_send("Errore: Registrazione modulo FAST fallita.");
    }

    return 0;
}
