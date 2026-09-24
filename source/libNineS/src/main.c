#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>

#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

// Includiamo l'header del loader nativo presente nel tuo repository
#include "onion/elfldr.h"

#include "ps5/mdbg.h"

#include <dlfcn.h>

// INCLUSIONE DELL'ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

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
        return false;
    }

    return success;
}

// 2. FUNZIONE DI CARICAMENTO DIRETTO IN RAM VIA ELFLDR (ANTI-PANIC)
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Mappatura A53 FAST in RAM...");
    usleep(1500000); // Pausa grafica di respiro

    // Otteniamo il PID del processo corrente (il demone di LiteHEN)
    pid_t local_pid = getpid();

    // 🚀 CHIAMATA DEFINITIVA: Usiamo l'elfldr nativo passando i 2 argomenti corretti.
    // Questo mappa l'array a53_ppr_install_fast_elf direttamente nello spazio di esecuzione
    // senza usare inject_elf o scrivere file su disco, bypassando i blocchi di sicurezza.
    intptr_t res = elfldr_load(local_pid, (uint8_t *)a53_ppr_install_fast_elf);

    if (res >= 0) {
        notify_send("LiteHEN: Modulo A53 FAST avviato in memoria!");
    } else {
        // Fallback estremo sulla Toolbox originale se il loader restituisce un errore
        if (Inject_Toolbox(0, (uint8_t *)a53_ppr_install_fast_elf)) {
            notify_send("LiteHEN: Modulo A53 FAST agganciato via Toolbox.");
        } else {
            notify_send("Errore: Impossibile mappare le patch KStuff.");
        }
    }
}

// 3. INTERRUTTORE DI INIZIALIZZAZIONE (Chiamato dopo i 5 secondi configurati in main.cpp)
int init_nineS(void)
{
    // Avvia il caricamento in-memory sicuro
    Start_ShadowMount_Embedded();

    return 0;
}
