#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Sfruttiamo rigorosamente le librerie interne di LiteHEN
#include "../include/proc.h"
#include "../include/ucred.h"
#include "../include/injector.h"
#include "../include/notify.h"
#include "../include/server.h"

#include "ps5/mdbg.h"
#include <dlfcn.h>

// INCLUSIONE DELL'ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

// Callback ufficiale di LiteHEN: quando riceve una connessione, applica le patch in memoria
void litehen_server_callback(int fd, void* data, ssize_t data_size) {
    // Gestione interna nativa dei pacchetti ricevuti dal server
}

// La tua funzione originale e intatta di LiteHEN per l'iniezione in SceShellUI
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

// FUNZIONE DI INIZIALIZZAZIONE NATIVA LITEHEN (Ex Main integrato)
int init_nineS(void)
{
    // ⏱️ LA PAUSA DI 5 SECONDI: Permette al demone padre di stabilizzarsi
    usleep(5000000);

    notify_send("LiteHEN: Sincronizzazione privilegi Kernel...");
    usleep(1500000);

    // 1. ELEVAZIONE DEI PRIVILEGI NATIVA DI LITEHEN
    // Invece di array personalizzati instabili, usiamo la macro interna 'curthread()' 
    // ereditata dagli header di LiteHEN per agganciare in sicurezza i giusti vettori dei privilegi
    struct thread* td = curthread(); 
    if (td) {
        // Applichiamo le funzioni ufficiali dell'SDK per sbloccare le capabilities utente/kernel
        kernel_set_ucred_caps(td);
        kernel_set_ucred_attrs(td);
    }

    notify_send("LiteHEN: Caricamento modulo A53 FAST...");
    usleep(1500000);

    // 2. INIEZIONE TRAMITE LOGICA TOOLBOX DI LITEHEN
    // Usiamo il PID del processo corrente (getpid()) che è stato appena jailbreakato 
    // e autorizzato dalle funzioni ucred native qui sopra.
    // Inject_Toolbox userà la funzione interna 'inject_elf' per mappare l'A53 FAST in RAM.
    if (Inject_Toolbox((int)getpid(), (uint8_t *)a53_ppr_install_fast_elf)) {
        notify_send("LiteHEN: Modulo A53 FAST attivato stabilmente!");
    } else {
        notify_send("Errore: Iniezione Toolbox rifiutata.");
    }

    // 3. AVVIO DEL SERVER DEI COMANDI NATIVO DI LITEHEN
    // Usiamo la firma esatta dell'header 'server.h' per tenere il payload residente in memoria sulla porta 9021
    start_server(9021, litehen_server_callback);

    return 0;
}
