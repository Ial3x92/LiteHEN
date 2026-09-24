#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Sfruttiamo esclusivamente le librerie interne e native di LiteHEN
#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

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
            notify_send("ELF failed to inject!");
        
        free(target_proc);
    }
    else{
        notify_send("unable to find process");
        return false;
    }

    return success;
}

// FUNZIONE DI INIZIALIZZAZIONE CON STRUTTURA ED ENTITY ORIGINALI (PID 1)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare e stabilizzare LiteHEN al 100%
    usleep(5000000);

    notify_send("LiteHEN pronto! Iniezione modulo A53 FAST...");
    usleep(1500000); // Pausa di respiro grafica

    // 🚀 L'ESECUZIONE NATIIVA SUL PID 1 🚀
    // Chiamiamo la tua Inject_Toolbox originale passandole il PID 1.
    // Avviene interamente in RAM, senza conflitti di rete sulla porta 9021 e senza toccare il disco.
    if (Inject_Toolbox(1, (uint8_t *)a53_ppr_install_fast_elf)) {
        notify_send("LiteHEN: Modulo A53 FAST attivato con successo!");
    } else {
        notify_send("Errore: Iniezione Toolbox fallita sul PID 1.");
    }

    return 0;
}
