#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"

// Includiamo l'header locale dell'elfldr
#include "onion/elfldr.h"

#include "ps5/mdbg.h"
#include <dlfcn.h>

// INCLUSIONE DEL NUOVO ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

// Funzione originale per l'iniezione in SceShellUI
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

// FUNZIONE DI AVVIO SICURO ANTI-KP: Iniezione diretta in memoria RAM
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Iniezione diretta A53 KStuff in memoria...");

    // Otteniamo il PID del processo del demone corrente (sicuro, locale e anti-KP)
    pid_t pid = getpid();

    // Usiamo elfldr_load con la firma a due argomenti corretta dal tuo header (PID, ELF)
    intptr_t res = elfldr_load(pid, (uint8_t *)a53_ppr_install_fast_elf);

    if (res >= 0) {
        notify_send("LiteHEN: Modulo A53 FAST caricato via elfldr!");
    } else {
        // Fallback usando la funzione di injection nativa se elfldr risponde picche
        if (Inject_Toolbox((int)pid, (uint8_t *)a53_ppr_install_fast_elf)) {
            notify_send("LiteHEN: Modulo A53 FAST iniettato localmente.");
        } else {
            notify_send("Errore: Fallito il caricamento di KStuff in RAM.");
        }
    }
}

// Inizializzazione unificata stabile
int init_nineS(void)
{
    // Lancia l'avvio sicuro in RAM del modulo A53/KStuff FAST
    Start_ShadowMount_Embedded();

    return 0;
}

