#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> // Necessario per la funzione chmod
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

// 2. FUNZIONE DI CARICAMENTO STABILE IN /data/ CON ARGOMENTI PASSED
void Start_ShadowMount_Embedded(void)
{
    // Percorso fisso e persistente nella memoria della console
    const char *temp_path = "/data/a53_kstuff_fast.elf";

    // CONTROLLO: Se il file non esiste, lo installiamo solo la prima volta
    if (access(temp_path, F_OK) != 0) {
        FILE *f = fopen(temp_path, "wb");
        if (!f) {
            notify_send("Errore: Cartella /data/ non accessibile.");
            return;
        }
        fwrite(a53_ppr_install_fast_elf, 1, a53_ppr_install_fast_elf_len, f);
        fclose(f);
        
        // Assegniamo i permessi di esecuzione completi (0777)
        chmod(temp_path, 0777);
        notify_send("LiteHEN: Prima installazione modulo A53 completata.");
        usleep(1000000);
    } else {
        notify_send("LiteHEN: Modulo A53 già presente in /data.");
        usleep(1000000);
    }

    notify_send("LiteHEN: Attivazione KStuff nel Kernel...");
    usleep(1500000);

    // 🚀 AVVIO CON ARGOMENTI COMPLETI IN BACKGROUND 🚀
    // Passiamo gli argomenti estratti dall'analisi dell'ELF originale.
    // Il simbolo '&' finale lancia il processo in background evitando blocchi al demone.
    system("/data/a53_kstuff_fast.elf --install --idle &");

    notify_send("LiteHEN: Modulo A53 FAST attivato!");
}

// 3. INTERRUTTORE DI INIZIALIZZAZIONE (Viene chiamato dopo i 5 secondi di attesa asincrona)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI
    // Permette a LiteHEN di completare l'avvio, applicare i settaggi e mostrare il benvenuto
    usleep(5000000);

    // Avvia il caricamento definitivo con i parametri passati
    Start_ShadowMount_Embedded();

    return 0;
}
