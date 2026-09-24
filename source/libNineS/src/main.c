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

// Questa funzione viene chiamata dal thread parallelo DOPO che LiteHEN si è stabilizzato
int init_nineS(void)
{
    notify_send("LiteHEN pronto! Caricamento A53 KStuff nel Kernel...");
    usleep(1500000); // Piccola pausa di sicurezza dopo la notifica

    // 🚀 INIEZIONE DIRETTA NEL KERNEL (PID 0) 🚀
    // Passiamo NULL come struct proc. La funzione inject_elf, vedendo il puntatore nullo,
    // applicherà l'array a53_ppr_install_fast_elf direttamente sulle tabelle di memoria globale del Kernel.
    if (inject_elf(NULL, (uint8_t *)a53_ppr_install_fast_elf)) {
        notify_send("LiteHEN: Modulo A53 FAST attivato nel Kernel con successo!");
    } else {
        // Fallback usando la Toolbox standard nel caso l'SDK richieda una inizializzazione esplicita
        if (Inject_Toolbox(0, (uint8_t *)a53_ppr_install_fast_elf)) {
            notify_send("LiteHEN: Modulo A53 FAST agganciato via Toolbox.");
        } else {
            notify_send("Errore: Iniezione KStuff fallita nel Kernel.");
        }
    }

    return 0;
}
