<p align="center">
  <img src="assets/logo.png" alt="LiteHEN" height="128" width="128"/>
</p>

# OnionLiteHEN

**OnionLiteHEN** is a streamlined, lightweight modification of OnionHEN, designed for users who want a cleaner experience without unnecessary clutter.

This project is a derivative work based on `aydencharles/onionHEN` and is fully compliant with the **GNU General Public License v3**.

---

## ## 🎮 INSTALLATION:

Just send litehen.elf. Nothing else is required.

LITEHEN AIO INCLUDES A53-KSTUFF-BACKPORT BY SHARKS

---

## ⚠️ IMPORTANT NOTES

**BEFORE USING IT, DELETE THE ONIONHEN FOLDER FROM /DATA, OTHERWISE THE CONFIGURATIONS WILL NOT BE OVERWRITTEN.**

---

## 🚀 Features & Optimizations (LiteHEN Edition)

Compared to the standard build (OnionHEN), the LiteHEN edition has been completely re-engineered for maximum stability, fast performance, and a drastically reduced memory footprint on the PS5.

### 🧠 System & Performance Optimizations
* **Minimal RAM Usage:** Disabled heavy C++ compiler overhead by forcing `-fno-exceptions` (Exception Handling) and `-fno-rtti` (Run-Time Type Information). This strips out massive static data tables, ensuring the payload leaves the maximum amount of console RAM free.
* **Ultra-Lightweight Binary:** Enabled aggressive size optimization (`-Os`) paired with Link Time Optimization (`-flto`). Through full symbol stripping (`-s`) and Dead Code Elimination (`--gc-sections`), all unused PS5 SDK functions are discarded, cutting the executable size down to the bare minimum.
* **Rock-Solid Anti-Kernel Panic (Anti-KP):** Fortified system stability during exploit injection by enforcing `-fno-strict-aliasing` and `-fno-omit-frame-pointer`. This prevents Clang from making risky compiler assumptions when casting memory pointers (type-punning), eliminating unexpected Orbis/Prospero kernel crashes.

### 🎨 Clean & Responsive User Interface (UI)
The structure of `toolbox_xml.cpp` has been streamlined to make the settings menus instantaneous and highly responsive:
* **Merged PKG & Payloads:** Completely removed the standalone payload group. User payload injection is now seamlessly integrated into the primary PKG menu for a faster and cleaner navigation experience.
* **Direct Fan Control:** Removed the redundant master switch toggle. The numeric threshold input field is now fully active and immediately accessible right when you enter the system settings.
* **Stripped Visual Clutter:** Removed unnecessary background art, graphical icons, and secondary descriptions (`std::nullopt`) across main groups to drastically reduce UI rendering overhead.

### 🛑 Server FTP Removal (Trancio Netto)
To ensure the absolute minimum footprint, the build pipeline completely cuts the built-in FTP server:
* Automatically strips `ftp_server.cpp` from the CMake compile tree and empties its contents.
* Massively comments out all initialization functions (`ftp_server_start` and `start_ftp_server`) within the source code to avoid compilation errors and reduce weight.

---

## 🛠️ Credits & Acknowledgments

OnionHEN exists because of the PS5 homebrew and reverse-engineering community.

### Contributors
*   **[aydencharles](https://github.com/aydencharles)** - Creator of OnionHEN and active contributor for PS5 tweaks.
*   **[kvnhrt](https://github.com/kvnhrt)** - Contributor of scripts and tools for the community.

### Based on
* **etaHEN** — [LightningMods](https://github.com/lightningmods) and contributors; source base of this tree
* **GoldHEN** — [sitro](https://github.com/SiSTR0) and contributors; the PS4 all-in-one HEN this project takes after

## 👥 Credits & Acknowledgments

A special thank you to all the developers, researchers, and contributors of the PS4 and PS5 scene for their amazing work and for releasing open-source tools:

*   **[John Törnblom](https://github.com/john-tornblom)** - Main developer of the PS5 Payload SDK and numerous essential modules.
*   **[Modmycon](https://github.com/Modmycon)** - Resource and host for exploits and guides dedicated to home consoles.
*   **[LightningMods](https://github.com/lightningmods)** - Lead developer of etaHEN, ItemzFlow, and the PS4 Homebrew Store.
*   **[EchoStretch](https://github.com/EchoStretch)** - Creator of PS5 App Dumper and developer of Kstuff/HEN ports.
*   **[earthonion](https://github.com/earthonion)** - Developer of garlic-savemgr and save managers.
*   **[itsPLK](https://github.com/itsPLK)** - Developer of the PS5 PKG Manager and related utilities.
*   **[drakmor](https://github.com/drakmor)** - Crucial researcher and contributor for PS5 kernel exploits.
*   **[xbcyl813](https://github.com/xbcyl813)** - Technical contributor and enthusiast of the modding scene.
*   **[ArkSama](https://github.com/ArkSama)** - Active developer in the Webkit exploit panorama.
*   **[matem6](https://github.com/matem6)** - Developer of network utilities and exploit configurations.
*   **[sitro](https://github.com/SiSTR0)** - Known for contributions to DNS servers and exploit redirection.
*   **[raw13g](https://github.com/raw13g)** - Contributor of automation scripts for payload loading.
*   **[oresterosso](https://github.com/oresterosso75)** - Active developer and beta tester in the Italian and international community.
*   **[bucanero](https://github.com/bucanero)** - Creator of Apollo Save Tool and historical preservation tools.
*   **[kerrdec97](https://github.com/kerrdec97)** - Researcher and developer of web interfaces for exploits.
*   **[Al Azif](https://github.com/al-azif)** - Developer of the famous DNS host and historical archive exploits for PS4.
*   **[PSBrew](https://github.com/PSBrew)** - Group and repository dedicated to the preservation and documentation of exploits.
*   **[soniciso1](https://github.com/soniciso1)** - Developer and tester of backup managers and ISO tools.
*   **[SHARKSIT](https://github.com/SHARKSIT)** - Creator A53.Kstuff.Backpork.elf

---

## ⚖️ License & Disclaimer

This project is licensed under the **GNU General Public License v3.0**. Third-party components retain their respective licenses and notices.

OnionHEN is an unofficial homebrew project and is not affiliated with Sony Interactive Entertainment. Use it only on hardware you own and at your own risk. No warranty is provided.
