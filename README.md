<p align="center">
  <img src="assets/logo.png" alt="OnionHEN" height="128" width="128"/>
</p>

# LiteHEN

LiteHEN è una modifica snella e leggera di **OnionHEN**, progettata per offrire un'esperienza utente pulita e priva di elementi superflui. 

Questo progetto è un'opera derivata basata su [aydencharles/onionHEN](https://github.com/aydencharles/onionHEN) ed è pienamente conforme alla licenza **GNU General Public License v3**.

## 🚀 Modifiche Principali & Cambiamenti

Rispetto all'originale OnionHEN, questa versione introduce le seguenti modifiche:
* **Overlay Riposizionato:** L'overlay principale/HUD è stato spostato nell'angolo **in alto a sinistra** dello schermo per una migliore visibilità e una minore ostruzione durante il gioco.
* **UI Semplificata:** Tutte le voci di debug e le opzioni ridondanti sono stato rimosse dai menu per focalizzare l'interfaccia utente esclusivamente su quattro sezioni pulite: **Installazione e gestione dei contenuti**, **Payload**, **Sistema e hardware** e **Informazioni**.
* **Focus sulle Prestazioni:** Sono stati mantenuti solo i componenti essenziali richiesti per l'iniezione dei payload e l'avvio degli homebrew.

## 🛠️ Crediti e Riconoscimenti

LiteHEN e OnionHEN esistono grazie al duro lavoro e alla dedizione della comunità di homebrew e reverse-engineering di PlayStation 5.

### Fondamenta Upstream & Ispirazione
* **aydencharles:** Per lo stack e codice di riferimento originale [aydencharles/onionHEN](https://github.com/aydencharles/onionHEN).
* **LightningMods:** Sviluppatore capo dell'ecosistema e del payload AIO [LightningMods/etaHEN](https://github.com).
* **SiSTR0:** Sviluppatore del celebre [SiSTR0/GoldHEN](https://github.com) per PS4, che ha ispirato l'ecosistema di questo progetto.

### Progetti di Riferimento
* **EchoStretch & sleirsgoevy:** Sviluppatori del nucleo kstuff e del caricamento dinamico disponibili su [EchoStretch/kstuff-lite](https://github.com). Utilizzato per il ripristino in Rest Mode (tramite monitoraggio `SceSysCore NOTE_EXEC` per `NPXS40087` e attendendo `libSceNpTrophy.sprx` / `libSceNpTrophy2.sprx`).
* **itsplk:** Sviluppatore di [itsPLK/ps5-payload-manager](https://github.com), da cui è mutuata la logica di rebind del listen-socket dopo la Rest Mode.
* **TeeKay87:** Curatore e gestore della raccolta globale di trucchi [TeeKay87/HEN-Cheats-Collection](https://github.com).
* **ArkSama:** Sviluppatore di [ArkSama/PHU-Games-Tools](https://github.com), le cui logiche di skip-hook e scansione `/dev/dce` hanno permesso l'implementazione del contatore FPS.

### Componenti Utilizzati o Incorporati
* **PS5 Payload SDK:** La toolchain Prospero e librerie di collegamento gestite su [JohnTornado/ps5-payload-sdk](https://github.com).
* **elfldr:** Caricatore di primo livello sulla porta **9021** ospitato su [sleirsgoevy/ps5-elfldr](https://github.com) (non incluso direttamente nel payload).
* **drakmor:** Sviluppatore della versione ottimizzata del server FTP integrato disponibile su [drakmor/ftpsrv](https://github.com).
* **astrelsky:** Creatore della libreria nativa per il patching dei processi e R/W [astrelsky/libhijacker](https://github.com/astrelsky/libhijacker).
* **buzzer-re:** Sviluppatore dello strumento di iniezione dei binari ELF nella ShellUI [buzzer-re/NineS](https://github.com/buzzer-re/NineS).
* **cJSON:** Libreria di parsing JSON ultraleggera disponibile su [DaveGamble/cJSON](https://github.com).
* **miniz:** Libreria a file singolo per la decompressione zip recuperabile su [richgel999/miniz](https://github.com).
* **7-Zip LZMA SDK:** SDK ufficiale per l'algoritmo di decompressione LZMA dell'unpacker.

## ⚖️ Licenza

Questo progetto è rilasciato sotto licenza **GNU General Public License v3 (GPL-3.0)** - consulta il file [LICENSE](LICENSE) per tutti i dettagli. I componenti di terze parti mantengono le rispettive licenze e note informative dei canali ufficiali.
