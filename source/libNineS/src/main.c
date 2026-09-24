#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> // 🏁 AGGIUNTO: Necessario per la funzione chmod
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"

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

// FUNZIONE DI ESTRAZIONE E AVVIO ANTI-KERNEL PANIC
void Start_ShadowMount_Embedded(void)
{
    // Percorso fisso e stabile in /data
    const char *temp_path = "/data/a53_kstuff_fast.elf";

    // CONTROLLO: Se il file non esiste, lo installiamo la prima volta
    if (access(temp_path, F_OK) != 0) {
        FILE *f = fopen(temp_path, "wb");
        if (!f) {
            notify_send("Errore: Impossibile installare il modulo in /data/");
            return;
        }
        
        // Scrive i byte dell'ELF FAST generati da PowerShell / xxd
        fwrite(a53_ppr_install_fast_elf, 1, a53_ppr_install_fast_elf_len, f);
        fclose(f);
        
        // Applica i permessi di esecuzione corretti al file appena creato
        chmod(temp_path, 0777);
        notify_send("LiteHEN: Installazione modulo A53 completata.");
    }

    // 🚀 AVVIO SICURO ANTI-KP: Eseguiamo il payload nativamente tramite system() 🚀
    // Questo evita i bug di memoria legati al fork()
    notify_send("LiteHEN: Avvio del modulo A53 KStuff...");
    
    // Esegue il comando in background lasciando intatto il demone principale
    system("/data/a53_kstuff_fast.elf --install --idle &");
    
    notify_send("LiteHEN caricato con successo!");
}

// Inizializzazione unificata priva di conflitti ucred
int init_nineS(void)
{
    // Lancia l'avvio sicuro del modulo A53/KStuff FAST
    Start_ShadowMount_Embedded();

    return 0;
}
