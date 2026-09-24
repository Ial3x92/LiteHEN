#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h> // Garantisce la funzione free()

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

// 2. FUNZIONE DI AVVIO IN RAM SU PROCESSI DI SISTEMA ESTERNI
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Caricamento modulo A53 FAST...");

    bool iniettato = false;
    
    // Lista dei PID standard utilizzati da SceShellUI / Processi di Sistema grafici su PS5
    int target_pids[] = {81, 82, 80, 83};
    int num_pids = sizeof(target_pids) / sizeof(target_pids[0]);

    for (int i = 0; i < num_pids; i++) {
        struct proc* system_proc = get_proc_by_pid(target_pids[i]);
        if (system_proc) {
            // Tenta l'iniezione in RAM sul processo esterno individuato
            if (inject_elf(system_proc, (uint8_t *)a53_ppr_install_fast_elf)) {
                iniettato = true;
                free(system_proc);
                break; // Iniezione riuscita, usciamo dal ciclo
            }
            free(system_proc);
        }
    }

    if (iniettato) {
        notify_send("LiteHEN: Modulo A53 FAST attivato in ShellUI!");
    } else {
        // Ultimo tentativo disperato: se i PID fissi falliscono, usiamo la tua funzione Inject_Toolbox
        // passando un valore convenzionale se supportato dal fallback interno
        if (!Inject_Toolbox(81, (uint8_t *)a53_ppr_install_fast_elf)) {
            notify_send("Errore: Iniezione KStuff fallita su tutti i vettori.");
        }
    }
}

// 3. INTERRUTTORE DI INIZIALIZZAZIONE
int init_nineS(void)
{
    // Lancia l'iniezione mirata nei processi di sistema esterni
    Start_ShadowMount_Embedded();

    return 0;
}
