<p align="center">
  <img src="assets/logo.png" alt="OnionHEN" height="128" width="128"/>
</p>

# LiteHEN

LiteHEN is a streamlined, lightweight modification of **OnionHEN**, designed for users who want a cleaner experience without unnecessary clutter.

This project is a derivative work based on [aydencharles/onionHEN](https://github.com/aydencharles/onionHEN) and is fully compliant with the **GNU General Public License v3**.

## 🚀 Key Modifications & Changes

Compared to the original OnionHEN, this version introduces the following changes:
* **Repositioned Overlay:** Moved the main overlay/HUD to the **top-left** corner of the screen for better visibility and less gameplay obstruction.
* **Streamlined UI:** Focus on three clean sections: **PKG and Payload**, **System and hardware**, and **Information**.
* **Performance Focus:** Section

## 🛠️ Credits & Acknowledgments

OnionHEN exists because of the PS5 homebrew and reverse-engineering community.

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

OnionHEN is an unofficial homebrew project and is not affiliated with Sony Interactive Entertainment. Use it only on hardware you own and at your own risk. No warranty is provided.
