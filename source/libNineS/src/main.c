#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Includiamo l'header del mini-loader nativo presente nel tuo repository
#include "onion/elfldr.h"

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

// La tua funzione originale e intatta rimane qui per non rompere i collegamenti del build
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

// FUNZIONE DI INIZIALIZZAZIONE CON CARICAMENTO IN RAM NATIVO ED EVACUAZIONE PORTA 9021
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare e stabilizzare LiteHEN al 100%
    usleep(5000000);

    notify_send("LiteHEN pronto! Caricamento A53 FAST in RAM...");
    usleep(1500000); // Pausa di respiro grafica

    // 🚀 CARICAMENTO NATIVO VIA ELFLDR (ANTI-KERNEL PANIC) 🚀
    // Passiamo il nostro getpid() corrente come processo ospite.
    // Questo alloca uno spazio di memoria sicuro e lancia l'A53 in background,
    // senza conflitti di rete sulla porta 9021 e senza toccare processi bloccati.
    intptr_t res = elfldr_load(getpid(), (uint8_t *)a53_ppr_install_fast_elf);

    if (res >= 0) {
        notify_send("LiteHEN: Modulo A53 FAST avviato con successo!");
    } else {
        notify_send("Errore: Il loader di memoria ha rifiutato l'A53.");
    }

    return 0;
}
