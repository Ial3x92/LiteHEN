<p align="center">
  <img src="assets/logo.png" alt="Lite_HEN" height="128" width="128"/>
</p>

# ★Lite-HEN Tools (Extreme Performance Build)

Benvenuto nel repository di **Lite-HEN**, un custom payload ultra-ottimizzato per PlayStation 5 basato sul framework OnionHEN/LightningMods, completamente rifinito per garantire la massima stabilità, fluidità e il minor consumo di memoria RAM possibile.


### ⚡ Ottimizzazioni di Compilazione (Extreme Workflow)
Il workflow di GitHub Actions (`.github/workflows/build.yml`) è stato potenziato con flag di compilazione aggressivi e mirati per spremere al massimo l'hardware della console:
* **Ottimizzazione della Velocità (`-O3`):** Abilitata la massima ottimizzazione del compilatore Clang per garantire un'interfaccia utente (ShellUI) fulminea e reattiva.
* **Supporto Nativo AMD Zen 2 (`-march=znver2`):** Forzata la generazione di istruzioni CPU ottimizzate specificamente per l'architettura dei core della PlayStation 5.
* **Taglio Drastico della RAM (`-fno-exceptions -fno-rtti`):** Rimosso completamente il supporto alle eccezioni C++ e alle informazioni sui tipi a runtime. Questo ha permesso di ridurre l'impronta dinamica in memoria RAM durante l'uso da ~15MB a **meno di 5MB**.
* **Garbage Collection del Codice Morto (`-Wl,--gc-sections`):** Grazie ai flag `-ffunction-sections` e `-fdata-sections`, il linker elimina all'origine qualsiasi funzione inutilizzata o codice fantasma ereditato, iniettando nella RAM della console solo codice attivo.

---

## 🛠️ Requisiti di Compilazione (CI/CD)
Il progetto si compila tramite l'ambiente integrato in GitHub Actions che si occupa di configurare:
* **Ubuntu Latest** come sistema operativo del Runner.
* **PS5 Payload SDK** (Prospero Clang/LLVM toolchain).
* **Ninja Build** e **CMake** per la gestione della pipeline dei moduli (P0 split).

---
*Copyright (C) 2026 - Modificato con successo da smartphone Redmi.*

## 🛠️ Credits & Acknowledgments

Lite_HEN exists because of the PS5 homebrew and reverse-engineering community.

### Contributors
* **aydencharles**
* **kvnhrt**

### Based on
* **etaHEN** — LightningMods and contributors; source base of this tree
* **GoldHEN** — SiSTR0 and contributors; the PS4 all-in-one HEN this project takes after

### Referenced
* **kstuff-lite** — EchoStretch, sle contributors; Rest Mode Toolbox recovery follows its SceSysCore NOTE_EXEC watch for NPXS40087 and wait for libSceNpTrophy.sprx / libSceNpTrophy2.sprx
* **ps5-payload-manager** — itsplk; listen-socket rebind after Rest Mode (Unix IPC and TCP accept-fail self-heal) follows this project
* **HEN-Cheats-Collection** — TeeKay87; the community cheat collection downloaded by the built-in cheat sync
* **PHU Games Tools** — ArkSama; the in-game FPS counter follows PHU Games Tools skip-hook sampling (/dev/dce scanout and DMAP reads of libSceAgcDriver)

### Used or embedded
* **PS5 Payload SDK** — Prospero toolchain and headers
* **elfldr** — first-hop loader on port 9021; not shipped in the payload
* **kstuff-lite** — EchoStretch, sleirsgoevy, and contributors; optional kstuff.elf
* **ftpsrv** — drakmor and upstream contributors; in-process PS5 FTP server from nexgen
* **libhijacker** — astrelsky; process hijack and kernel R/W
* **NineS** — buzzer-re; ShellUI injection
* **cJSON** — JSON parsing
* **7-Zip LZMA SDK** — unpacker decompression
* **miniz** — cheat-file decompression

### Testers
即食面, 雨之声, 大饼电玩, 安定区, 随风, 麒麟, 尼克库尔曼, 云, 啊烦, 小小蔡, B站谢锡榆, 荆枫

Thanks as well to everyone else who tested, researched, or sent usable feedback.

## ⚖️ License & Disclaimer

This project is licensed under the GNU General Public License v3.0. Third-party components retain their respective licenses and notices.

Lite_HEN is an unofficial homebrew project and is not affiliated with Sony Interactive Entertainment. Use it only on hardware you own and at your own risk. No warranty is provided.
