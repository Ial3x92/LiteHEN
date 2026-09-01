/* Copyright (C) 2026 LiteHEN / LightningMods */
#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void bootstrap_notify(const char *text, ...);
void bootstrap_notify_starting(bool custom_icon_ready);

#ifdef __cplusplus
}
#endif
