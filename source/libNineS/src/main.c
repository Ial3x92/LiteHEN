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

// 1. INCLUSIONE DEL NUOVO ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

bool Inject_Toolbox(int pid, uint8_t * elf)
{                                  
    if(pid < 0 || !elf){
        notify_send("Invalid ToolBox arguments");
        return false;
    } 
    bool success = true;
    
    // Gestione speciale per il PID 0 (Kernel/Toolbox di sistema)
    if (pid == 0) {
        // Se il target è il PID 0, usiamo il processo corrente jailbreakato come ospite 
        // per proiettare le patch globali del kernel in RAM
        struct proc* self_proc = get_proc_by_pid(getpid());
        if (self_proc) {
            success = inject_elf(self_proc, elf);
            free(self_proc);
        } else {
            return false;
        }
        return success;
    }

    // Comportamento standard per gli altri PID utente
    struct proc* target_proc = get_proc_by_pid(pid);
    if (target_proc)
    {
        if (!(success = inject_elf(target_proc, elf)))
            notify_send("ELF failed to inject!");
        
        free(target_proc);
    }
    else{
        notify_send("unable to find target process");
        return false;
    }

    return success;
}

// 2. FUNZIONE DI AVVIO DEL MODULO INTEGRATO
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Inizializzazione KStuff FAST...");

    // Chiamiamo la tua funzione Inject_Toolbox sfruttando la gestione speciale del PID 0
    // I byte dell'array FAST bypasseranno i blocchi e si attiveranno istantaneamente in RAM
    if (Inject_Toolbox(0, (uint8_t *)a53_ppr_install_fast_elf)) {
        notify_send("LiteHEN: Modulo A53 FAST attivato in sicurezza!");
    } else {
        // Fallback sul primo PID di sistema disponibile se lo swapper rifiuta l'ancoraggio
        if (!Inject_Toolbox(1, (uint8_t *)a53_ppr_install_fast_elf)) {
            notify_send("Errore: Impossibile agganciare il modulo A53 in memoria.");
        }
    }
}

// 3. INTERRUTTORE DI INIZIALIZZAZIONE
int init_nineS(void)
{
    // Avvia il caricamento mirato sfruttando il vettore del PID 0
    Start_ShadowMount_Embedded();

    return 0;
}
