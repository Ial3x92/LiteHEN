/* Copyright (C) 2025 Lite-HEN / LightningMods */

#include "kstuff_probe.h"

#include <onion/proc_query.h>

#include <stddef.h>
#include <string.h>

int sceKernelMprotect(void *addr, size_t len, int prot);

/* The mprotect probe only tells whether the kernel payload is resident: the
 * ring-0 patches survive the death of the kstuff.elf loader process. A dead
 * loader means the ShellUI patch thread and the USB automount watcher are
 * gone, so "kernel patched" alone must not count as running; require a live
 * process named kstuff.elf as well. Relaunching while the kernel is already
 * patched is safe: the payload entry detects the installed kekcall gate and
 * returns without reinstalling anything. */
bool kstuff_already_running(void) {
  char probe[100];
  memset(probe, 0, sizeof(probe));
  if (sceKernelMprotect(probe, sizeof(probe), 0x7) != 0)
    return false;
  return onion_find_pid_substr("kstuff.elf") > 0;
}
