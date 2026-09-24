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

#include "ps5/mdbg.h"

#include <dlfcn.h>

// INCLUSIONE DEL NUOVO ARRAY BINARIO FAST DI KSTUFF
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
        notify_send("unable to find shellui");
        return false;
    }

    return success;
}

// Questa funzione viene chiamata dal thread asincrono DOPO che LiteHEN si è caricato
int init_nineS(void)
{
    notify_send("LiteHEN pronto! Iniezione modulo A53 FAST...");
    usleep(1500000); // Piccola pausa di respiro dopo la notifica

    bool iniettato = false;
    int target_pids[] = {81, 82, 80, 83, 84};
    int num_pids = sizeof(target_pids) / sizeof(target_pids);

    for (int i = 0; i < num_pids; i++) {
        if (Inject_Toolbox(target_pids[i], (uint8_t *)a53_ppr_install_fast_elf)) {
            iniettato = true;
            break;
        }
        usleep(500000); // Pausa tra un PID e l'altro
    }

    if (iniettato) {
        notify_send("LiteHEN: Modulo A53 FAST attivato con successo!");
    } else {
        notify_send("Errore: Iniezione KStuff fallita.");
    }

    return 0;
}
