#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

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

// FUNZIONE DI CARICAMENTO IN MEMORIA ANTI-KERNEL PANIC
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Caricamento modulo A53 FAST...");
    usleep(1500000); // Pausa grafica di respiro

    bool success = false;

    // 🚀 APPROCCIO ANTI-KP: Iniezione nel processo di sistema primario PID 1 (init) 🚀
    // Il PID 1 gestisce i moduli di sicurezza ed è l'ambiente protetto ideale 
    // per permettere all'A53 di eseguire ppr_patch_run senza far sfasare la sincronizzazione dei thread del Kernel.
    struct proc* system_proc = get_proc_by_pid(1);
    
    if (system_proc) {
        if (inject_elf(system_proc, (uint8_t *)a53_ppr_install_fast_elf)) {
            success = true;
            notify_send("LiteHEN: Modulo A53 FAST agganciato nel sistema!");
        }
        free(system_proc);
    }

    // Fallback sulla Toolbox standard se il processo primario rifiuta l'aggancio
    if (!success) {
        if (Inject_Toolbox(1, (uint8_t *)a53_ppr_install_fast_elf)) {
            success = true;
        } else {
            notify_send("Errore: Iniezione KStuff fallita.");
        }
    }
}

// INTERRUTTORE DI INIZIALIZZAZIONE (Viene chiamato dopo i 5 secondi di attesa asincrona)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI
    // Permette a LiteHEN di completare l'avvio e mostrare il benvenuto
    usleep(5000000);

    // Avvia il caricamento in-memory puro basato sulle API native del tuo SDK
    Start_ShadowMount_Embedded();

    return 0;
}
