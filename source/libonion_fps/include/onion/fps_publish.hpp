/* Copyright (C) 2026 Lite_HEN / LightningMods */
#pragma once

#include <onion/fps_sample.h>

namespace onion {
namespace fps {

bool publish_open();
void publish_close();
void publish(const OnionFpsSample &sample);

} // namespace fps
} // namespace onion
