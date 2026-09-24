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

// Includiamo l'header dell'SDK per caricare gli ELF direttamente in memoria RAM
#include "elfldr_remote.h"

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

// FUNZIONE DI AVVIO SICURO ANTI-KP: Esegue il payload direttamente dalla RAM
void Start_ShadowMount_Embedded(void)
{
    notify_send("LiteHEN: Iniezione diretta A53 KStuff in memoria...");

    // Argomenti testuali per configurare il modulo A53 FAST
    const char *args[] = {"--install", "--idle", NULL};

    // 🚀 CARICAMENTO RETE/RAM NATIVO DELL'SDK 🚀
    // Questa funzione prende i byte grezzi dall'array a53_embedded.h e li mappa 
    // istantaneamente in un nuovo processo utente sicuro senza passare per il disco (/data o /tmp).
    int res = elfldr_rem_load(a53_ppr_install_fast_elf, a53_ppr_install_fast_elf_len, args);

    if (res == 0) {
        notify_send("LiteHEN: Modulo A53 FAST caricato in RAM con successo!");
    } else {
        // Se la chiamata remota fallisce, proviamo l'esecuzione locale diretta
        res = elfldr_load(a53_ppr_install_fast_elf, a53_ppr_install_fast_elf_len, args);
        if (res == 0) {
            notify_send("LiteHEN: Modulo A53 FAST avviato localmente.");
        } else {
            notify_send("Errore: Impossibile mappare il payload KStuff in RAM.");
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
