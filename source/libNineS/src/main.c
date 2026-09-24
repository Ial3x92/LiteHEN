#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h> // Aggiunto per garantire la funzione free()

#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

#include "ps5/mdbg.h"

#include <dlfcn.h>

// 🏁 1. INCLUSIONE DEL NUOVO ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

bool Inject_Toolbox(int pid, uint8_t * elf)
{                                  
    if(pid < 0 || !elf){
        notify_send("Invalid ToolBox arguments");
        return false;
    } 
    bool success = true;
    struct proc* target_proc = get_proc_by_pid(pid);//find_proc_by_name("SceShellUI");
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

// 🏁 2. FUNZIONE AGGIUNTA PER LANCIARE IL MODULO A53 IN RAM (ANTI-KP)
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Caricamento modulo A53 FAST...");

    // Otteniamo la struttura del processo corrente del demone in modo nativo e sicuro
    struct proc* self_proc = get_proc_by_pid(getpid());
    
    if (self_proc) {
        // Iniettiamo i byte estratti dall'header FAST direttamente nello spazio di memoria RAM
        if (inject_elf(self_proc, (uint8_t *)a53_ppr_install_fast_elf)) {
            notify_send("LiteHEN: Modulo A53 FAST attivato in memoria!");
        } else {
            notify_send("Errore: Iniezione locale di KStuff fallita.");
        }
        free(self_proc);
    } else {
        notify_send("Errore: Impossibile mappare il processo demone corrente.");
    }
}

// 🏁 3. INTERRUTTORE DI INIZIALIZZAZIONE (Rinominato da main per evitare duplicati nel linker)
int init_nineS(void)
{
    // Lancia l'iniezione sicura in RAM del modulo A53/KStuff FAST
    Start_ShadowMount_Embedded();

    return 0;
}
