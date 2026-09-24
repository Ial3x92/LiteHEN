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
* aydencharles
* kvnhrt

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

*Thanks as well to everyone else who tested, researched, or sent usable feedback.*

---

## ⚖️ License & Disclaimer

This project is licensed under the **GNU General Public License v3.0**. Third-party components retain their respective licenses and notices.

OnionHEN is an unofficial homebrew project and is not affiliated with Sony Interactive Entertainment. Use it only on hardware you own and at your own risk. No warranty is provided.
