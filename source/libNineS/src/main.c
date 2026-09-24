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

// INCLUSIONE DELL'ARRAY BINARIO INCORPORATO DI KSTUFF / BACKPORK
#include "a53_embedded.h" 

// Callback fittizia per soddisfare i requisiti del server di rete nell'SDK aggiornato
void dummy_server_callback(int fd, void* data, ssize_t data_size) {
    // Gestione pacchetti vuota (personalizzabile se necessario)
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

// FUNZIONE DI ESTRAZIONE E AVVIO: Estrae ed esegue l'ELF direttamente dalla RAM di LiteHEN
void Start_ShadowMount_Embedded(void)
{
    // Creiamo un percorso temporaneo nella RAM volatile della console (/tmp viene svuotata al riavvio)
    const char *temp_path = "/tmp/a53_kstuff_temp.elf";

    // 1. Scrittura del file temporaneo prendendo i dati dall'array incorporato
    FILE *f = fopen(temp_path, "wb");
    if (!f) {
        notify_send("Errore: Impossibile creare il file temporaneo in /tmp/");
        return;
    }
    
    // CORRETTO: Scrive i byte dell'ELF usando il nome dell'array reale estratto da a53_embedded.h
    fwrite(a53_ppr_install_compact, 1, sizeof(a53_ppr_install_compact), f);
    fclose(f);

    // 2. Creiamo il processo figlio parallelo tramite fork()
    pid_t pid = fork();

    if (pid < 0) {
        notify_send("Errore critico durante il fork parallelo");
        unlink(temp_path); // Pulizia immediata in caso di errore di fork
        return;
    }

    if (pid == 0) {
        // -----------------------------------------------------------------
        // PROCESSO FIGLIO: Gira in background e attende la stabilizzazione
        // -----------------------------------------------------------------
        
        // Aspetta 10 secondi lasciando che LiteHEN (il padre) si carichi completamente per primo
        sleep(10); 
        
        notify_send("LiteHEN: Avvio diretto del modulo A53 KStuff...");
        
        // Prepariamo gli argomenti standard per l'eseguibile di Shadow Mount+
        char *args[] = {(char *)temp_path, "--install", "--idle", NULL};
        
        // Eseguiaimo il payload dall'indirizzo temporaneo
        execv(temp_path, args);
        
        // Se execv fallisce (ad esempio per problemi di permessi o firmware non supportato),
        // il processo figlio si chiude per evitare di bloccare il sistema
        exit(EXIT_FAILURE);
    } 
    else {
        // -----------------------------------------------------------------
        // PROCESSO PADRE: Il core di LiteHEN
        // -----------------------------------------------------------------
        
        // Ignora il segnale del figlio per evitare che diventi un processo "zombie" in memoria
        signal(SIGCHLD, SIG_IGN); 
        
        // Diamo mezzo secondo di tempo al figlio per registrare l'apertura dell'eseguibile,
        // dopodiché possiamo scollegare (unlink) il file temporaneo.
        usleep(500000); 
        unlink(temp_path); 

        notify_send("LiteHEN caricato! Modulo A53 iniettato internamente.");
    }
}

// CORRETTO: Rinominata la funzione per evitare conflitti con il main del daemon
int init_nineS(void)
{
    // Elevazione dei privilegi utente/kernel (ucred) necessaria su PS5
    struct thread* td = curthread(); 
    if (td) {
        // CORRETTO: Allocati gli array con le dimensioni richieste dall'SDK (16 e 32)
        uint8_t full_caps[16];
        uint8_t full_attrs[32];
        memset(full_caps, 0xFF, sizeof(full_caps));
        memset(full_attrs, 0xFF, sizeof(full_attrs));

        kernel_set_ucred_caps(0, full_caps);
        kernel_set_ucred_attrs(0, full_attrs);
    }

    notify_send("Welcome To LiteHEN All-In-One");

    // Lancia l'estrazione e l'esecuzione asincrona del modulo A53/KStuff
    Start_ShadowMount_Embedded();

    // CORRETTO: Inseriti i parametri obbligatori (Porta 9021 e la funzione di callback)
    start_server(9021, dummy_server_callback);

    return 0;
}
