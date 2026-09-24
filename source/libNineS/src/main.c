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
#include "../include/server.h"

#include "ps5/mdbg.h"
#include <dlfcn.h>

// INCLUSIONE DEL NUOVO ARRAY BINARIO FAST DI KSTUFF
#include "a53_embedded.h" 

// Callback fittizia per soddisfare i requisiti del server di rete nell'SDK aggiornato
void dummy_server_callback(int fd, void* data, ssize_t data_size) {
    // Gestione pacchetti vuota
}

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

// FUNZIONE DI ESTRAZIONE E AVVIO: Estrae ed esegue il modulo FAST
void Start_ShadowMount_Embedded(void)
{
    // Creiamo un percorso temporaneo nella RAM volatile della console
    const char *temp_path = "/tmp/a53_kstuff_temp.elf";

    // 1. Scrittura del file temporaneo prendendo i dati dall'array FAST
    FILE *f = fopen(temp_path, "wb");
    if (!f) {
        notify_send("Errore: Impossibile creare il file temporaneo in /tmp/");
        return;
    }
    
    // CORRETTO: Usa le variabili generate automaticamente da xxd per la versione FAST
    fwrite(a53_ppr_install_fast_elf, 1, a53_ppr_install_fast_elf_len, f);
    fclose(f);

    // 2. Creiamo il processo figlio parallelo tramite fork()
    pid_t pid = fork();

    if (pid < 0) {
        notify_send("Errore critico durante il fork parallelo");
        unlink(temp_path);
        return;
    }

    if (pid == 0) {
        // -----------------------------------------------------------------
        // PROCESSO FIGLIO: Background
        // -----------------------------------------------------------------
        sleep(10); 
        
        notify_send("LiteHEN: Avvio diretto del modulo A53 KStuff (FAST)...");
        
        // Prepariamo gli argomenti standard per l'eseguibile di Shadow Mount+
        char *args[] = {(char *)temp_path, "--install", "--idle", NULL};
        
        // Eseguiamo il payload dall'indirizzo temporaneo
        execv(temp_path, args);
        exit(EXIT_FAILURE);
    } 
    else {
        // -----------------------------------------------------------------
        // PROCESSO PADRE: Core LiteHEN
        // -----------------------------------------------------------------
        signal(SIGCHLD, SIG_IGN); 
        usleep(500000); 
        unlink(temp_path); 

        notify_send("LiteHEN caricato! Modulo A53 FAST iniettato.");
    }
}

// Inizializzazione unificata di libNineS senza conflitti di linker
int init_nineS(void)
{
    // Elevazione dei privilegi utente/kernel (ucred)
    struct thread* td = curthread(); 
    if (td) {
        uint8_t full_caps[16];
        uint8_t full_attrs[32];
        memset(full_caps, 0xFF, sizeof(full_caps));
        memset(full_attrs, 0xFF, sizeof(full_attrs));

        kernel_set_ucred_caps(0, full_caps);
        kernel_set_ucred_attrs(0, full_attrs);
    }

    notify_send("Welcome To LiteHEN All-In-One");

    // Lancia l'estrazione e l'esecuzione asincrona del modulo FAST
    Start_ShadowMount_Embedded();

    // Avvia il server dei comandi di LiteHEN (porta 9021)
    start_server(9021, dummy_server_callback);

    return 0;
}
