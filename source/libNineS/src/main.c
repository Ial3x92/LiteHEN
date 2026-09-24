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

// La tua funzione originale e intatta di LiteHEN per l'iniezione in SceShellUI
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

// FUNZIONE DI INIZIALIZZAZIONE NATIVA LITEHEN (Sincronizzata con main.cpp)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Permette al demone principale di OnionHEN di fare il boot completo
    usleep(5000000);

    notify_send("LiteHEN: Caricamento modulo A53 FAST...");
    usleep(1500000);

    // 🚀 LOGICA NATIVA LITEHEN: Iniezione in RAM usando la Toolbox sul processo corrente (getpid())
    // I privilegi sono già stati elevati stabilmente dal main.cpp prima di questa chiamata.
    // Viene iniettato l'array esadecimale FAST senza toccare il disco per evitare Kernel Panic.
    if (Inject_Toolbox((int)getpid(), (uint8_t *)a53_ppr_install_fast_elf)) {
        notify_send("LiteHEN: Modulo A53 FAST attivato stabilmente!");
    } else {
        notify_send("Errore: Iniezione Toolbox rifiutata.");
    }

    return 0;
}
