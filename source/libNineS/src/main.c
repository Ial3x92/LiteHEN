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

// 1. INCLUSIONE DEL NUOVO ARRAY BINARIO FAST DI KSTUFF (Generato dai dati del dump)
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

// 2. FUNZIONE DI CARICAMENTO BASATA SUI DATI REALI DEL PAYLOAD
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Esecuzione modulo A53 FAST nel Kernel...");
    usleep(1500000); // Pausa grafica di respiro

    // Dati estratti dal binario: il file patcha direttamente le tabelle del Kernel (KERNEL_ADDRESS_TEXT_BASE)
    // Agisce come estensione del demone stesso sfruttando getpid() per scalare i privilegi utente/kernel.
    pid_t local_pid = getpid();
    struct proc* self_proc = get_proc_by_pid(local_pid);

    if (self_proc) {
        // Iniettiamo i byte FAST direttamente nella memoria RAM dell'ambiente protetto corrente
        // Il payload eseguirà internamente ppr_patch_run per sbloccare il Kernel Core
        if (inject_elf(self_proc, (uint8_t *)a53_ppr_install_fast_elf)) {
            notify_send("LiteHEN: Modulo A53 FAST caricato in RAM!");
        } else {
            // Fallback usando la Toolbox nativa sul PID 0 se il sistema richiede l'astrazione
            if (Inject_Toolbox(0, (uint8_t *)a53_ppr_install_fast_elf)) {
                notify_send("LiteHEN: Modulo A53 FAST agganciato via Toolbox.");
            } else {
                notify_send("Errore: Impossibile mappare le patch KStuff.");
            }
        }
        free(self_proc);
    } else {
        notify_send("Errore: Ambiente di runtime non pronto.");
    }
}

// 3. INTERRUTTORE DI INIZIALIZZAZIONE (Chiamato dopo i 5 secondi configurati in main.cpp)
int init_nineS(void)
{
    // Avvia l'esecuzione basata sui dati reali dell'array FAST
    Start_ShadowMount_Embedded();

    return 0;
}
