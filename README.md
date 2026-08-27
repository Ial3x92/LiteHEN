<p align="center">
  <img src="assets/logo.png" alt="OnionHEN" height="128" width="128"/>
</p>

# LiteHEN

LiteHEN is a streamlined, lightweight modification of **OnionHEN**, designed for users who want a cleaner experience without unnecessary clutter.

This project is a derivative work based on [aydencharles/onionHEN](https://github.com) and is fully compliant with the **GNU General Public License v3**.

## 🚀 Key Modifications & Changes

Compared to the original OnionHEN, this version introduces the following changes:
* **Repositioned Overlay:** Moved the main overlay/HUD to the **top-left** corner of the screen for better visibility and less gameplay obstruction.
* **Streamlined UI:** Removed all cluttered **debug entries** and options from the menus to provide a cleaner, user-friendly interface.
* **Performance Focus:** Kept only the essential components required for payload injection and homebrew launching.

## 🛠️ Credits & Acknowledgments

LiteHEN and OnionHEN exist thanks to the dedicated work of the PlayStation 5 homebrew and reverse-engineering community.

### Upstream Foundations & Inspiration
* **aydencharles:** For creating the original [OnionHEN](https://github.com) stack.
* **etaHEN:** Developed by [LightningMods](https://github.com) and contributors; serves as the base source code for this software tree.
* **GoldHEN:** Created by [SiSTR0](https://github.com) and contributors; the all-in-one PS4 HEN that inspired the ecosystem design of this project.

### Referenced Projects & Technology
* **kstuff-lite:** Developed by [EchoStretch](https://github.com), [sleirsgoevy](https://github.com), and contributors. Used for Rest Mode Toolbox recovery (following `SceSysCore NOTE_EXEC` monitoring for `NPXS40087` and waiting for `libSceNpTrophy.sprx` / `libSceNpTrophy2.sprx`).
* **ps5-payload-manager:** Created by [itsplk](https://github.com). The listen-socket rebind logic after Rest Mode (Unix IPC and TCP accept-fail self-heal) is based on this implementation.
* **HEN-Cheats-Collection:** Maintained by [TeeKay87](https://github.com); the community cheat catalog downloaded dynamically by the built-in cheat sync module.
* **PHU Games Tools:** Created by [ArkSama](https://github.com). The in-game FPS counter leverages their skip-hook sampling method (`/dev/dce` scanout and DMAP reads of `libSceAgcDriver`).

## ⚖️ License

This project is licensed under the **GNU General Public License v3 (GPL-3.0)** - see the [LICENSE](LICENSE) file for full details. Third-party components retain their respective licenses and notices.
