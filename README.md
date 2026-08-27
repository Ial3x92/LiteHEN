<p align="center">
  <img src="assets/logo.png" alt="OnionHEN" height="128" width="128"/>
</p>

# LiteHEN

LiteHEN is a streamlined, lightweight modification of **OnionHEN**, designed for users who want a cleaner experience without unnecessary clutter.

This project is a derivative work based on [aydencharles/onionHEN](https://github.com) and is fully compliant with the **GNU General Public License v3**.

## 🚀 Key Modifications & Changes

Compared to the original OnionHEN, this version introduces the following changes:
* **Repositioned Overlay:** Moved the main overlay/HUD to the **top-left** corner of the screen for better visibility and less gameplay obstruction.
* **Streamlined UI:** Removed all cluttered debug entries and options from the menus to focus exclusively on four clean sections: **Installazione e gestione dei contenuti**, **Payload**, **Sistema e hardware**, and **Informazioni**.
* **Performance Focus:** Kept only the essential components required for payload injection and homebrew launching.

## 🛠️ Credits & Acknowledgments

LiteHEN and OnionHEN exist because of the hard work and dedication of the PlayStation 5 homebrew and reverse-engineering community.

### Contributors
* **aydencharles:** Creator of the original [aydencharles/onionHEN](https://github.com) ecosystem.
* **kvnhrt:** Core contributor and developer [kvnhrt](https://github.com).
* OnionHEN contributors.

### Based on
* **etaHEN:** Developed by [LightningMods](https://github.com) and contributors; source base of this tree.
* **GoldHEN:** Created by [SiSTR0](https://github.com) and contributors; the PS4 all-in-one HEN this project takes after.

### Referenced
* **kstuff-lite:** Developed by [EchoStretch](https://github.com), [sleirsgoevy](https://github.com), and contributors. Rest Mode Toolbox recovery follows its `SceSysCore NOTE_EXEC` watch for `NPXS40087` and waits for `libSceNpTrophy.sprx` / `libSceNpTrophy2.sprx`.
* **ps5-payload-manager:** Created by [itsplk](https://github.com); listen-socket rebind after Rest Mode (Unix IPC and TCP accept-fail self-heal) follows this project.
* **HEN-Cheats-Collection:** Maintained by [TeeKay87](https://github.com); the community cheat collection downloaded by the built-in cheat sync.
* **PHU Games Tools:** Developed by [ArkSama](https://github.com); the in-game FPS counter follows PHU Games Tools skip-hook sampling (`/dev/dce` scanout and DMAP reads of `libSceAgcDriver`).

### Used or embedded
* **PS5 Payload SDK:** Prospero toolchain and headers hosted on [JohnTornado/ps5-payload-sdk](https://github.com).
* **elfldr:** First-hop loader on port **9021** hosted on [sleirsgoevy/ps5-elfldr](https://github.com/ps5-elfldr) (not shipped in the payload).
* **kstuff-lite:** Optional `kstuff.elf` payload components by [EchoStretch](https://github.com).
* **ftpsrv:** Built-in PS5 FTP server module from nexgen by [drakmor](https://github.com) and upstream contributors.
* **libhijacker:** Process hijack and kernel R/W library by [astrelsky](https://github.com).
* **NineS:** ShellUI injection utility developed by [buzzer-re](https://github.com).
* **cJSON:** JSON parsing framework from [DaveGamble/cJSON](https://github.com).
* **7-Zip LZMA SDK:** Unpacker decompression mechanisms.
* **miniz:** Single-file zip decompression headers from [richgel999/miniz](https://github.com).

### Testers
即食面, 雨之声, 大饼电玩, 安定区, 随风, 麒麟, 尼克库尔曼, 云, 啊烦, 小小蔡, B站谢锡榆, 荆枫

*Thanks as well to everyone else who tested, researched, or sent usable feedback.*

## ⚖️ License

This project is licensed under the **GNU General Public License v3 (GPL-3.0)** - see the [LICENSE](LICENSE) file for full details. Third-party components retain their respective licenses and notices.
