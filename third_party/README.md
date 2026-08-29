# Third-party dependencies

Everything in this directory is maintained outside Lite_HEN. Keeping external
code here makes `source/` exclusively first-party code.

| Path | Form | Role in Lite_HEN |
|------|------|------------------|
| [`7zip_sdk/`](7zip_sdk/) | Vendored source | LZMA support used by the unpacker |
| [`cjson/`](cjson/) | Vendored source | JSON parsing and serialization |
| [`cheat_support/`](cheat_support/) | Vendored source | AES, base64, miniz and SHA-256 used by cheat parsers |
| [`keystone/`](keystone/) | Headers + prebuilt archive | ShnExt assembly support |
| [`kstuff-lite/`](kstuff-lite/) | Git submodule | Produces the optional embedded `kstuff.elf` |
| [`ftpsrv/`](ftpsrv/) | Vendored source (`nexgen`) | PS5 FTP source module compiled into util |
| [`ShadowMountPlus/`](ShadowMountPlus/) | Vendored source (`1.6beta16`) | Game scanner/mounter source module compiled into util |
| [`sqlite/`](sqlite/) | Vendored amalgamation | Public-domain SQLite used by the ShadowMount+ module |

Third-party file names retain their upstream spelling even when it differs
from the project's snake_case convention. This keeps upstream updates easy to
review.

Source-built or downloaded fallback dependency blobs are cached in
`.cache/dependencies/` and ignored by Git. `scripts/sync_dependencies.sh`
stages the required bootstrapper input from that cache; generated blobs do not
belong in `source/`.

`ftpsrv` uses a pinned `nexgen` source revision and is compiled as a module in
`util.elf`. Its adapter exposes a stop latch so the util facade can restart the
listener when the user changes the port or disables the service.

`ShadowMountPlus` is vendored at the upstream `1.6beta16` release revision and
is also compiled as a module in `util.elf`; its standalone `main.c` stays out of
the build and a facade-owned worker thread drives the module through
`source/util/source/shadowmount_main.cpp`. The public-domain SQLite
amalgamation under `sqlite/` satisfies its app-database dependency because the
PS5 payload SDK does not ship libsqlite3.

## Runtime-only external dependency

[ps5-payload-dev/elfldr](https://github.com/ps5-payload-dev/elfldr) on port 9021
is supplied by the runtime environment for the initial bootstrap.

```bash
git submodule update --init --recursive
./scripts/sync_dependencies.sh
```
