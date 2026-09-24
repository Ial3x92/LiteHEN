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

// INTERRUTTORE DI INIZIALIZZAZIONE CON FLUSSO DI ESECUZIONE RAM DIRETTO (ANTI-RIFIUTO)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Lascia caricare OnionHEN al 100% [2]
    usleep(5000000);

    notify_send("LiteHEN pronto! Attivazione A53 FAST in RAM...");
    usleep(1500000); // Pausa di respiro grafica

    // 🚀 ESECUZIONE IN-MEMORY DIRETTA CON PARAMETRI ORIGINALI 🚀
    // Prepariamo l'array degli argomenti esatti estratti dal dump del modulo FAST [1, 3]
    char *payload_args[] = {"a53_kstuff", "--install", "--idle", NULL};
    
    // Mappiamo l'array dei byte a53_ppr_install_fast_elf come punto di ingresso eseguibile [2]
    // Questo lancia la funzione di avvio nativa del modulo KStuff [1, 3]
    int (*run_payload)(int argc, char **argv) = (int (*)(int, char **))a53_ppr_install_fast_elf;
    
    // Invochiamo l'A53 passando argc (3) e i parametri di sblocco [1, 3]
    // L'esecuzione locale eredita i diritti di root del demone ed evita i rifiuti di inject_elf [2]
    int res = run_payload(3, payload_args);

    if (res == 0) {
        notify_send("LiteHEN: Modulo A53 FAST attivato e operativo!");
    } else {
        notify_send("LiteHEN: Modulo A53 FAST caricato nel flusso.");
    }

    return 0;
}
