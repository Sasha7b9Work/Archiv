#pragma once

#define ColorFromRGB(r, g, b) {r << 16 | g << 8 | b }

static const int ColorWHITE = ColorFromRGB(255, 255, 255);
static const int ColorBLACK = ColorFromRGB(0, 0, 0);
static const int ColorR211 = ColorFromRGB(211, 197, 250);
static const int ColorR238 = ColorFromRGB(238, 119, 134);
static const int ColorR048 = ColorFromRGB(48, 24, 27);
static const int ColorR064 = ColorFromRGB(64, 64, 96);
static const int ColorR154 = ColorFromRGB(154, 154, 230);
static const int ColorR128 = ColorFromRGB(128, 128, 192);
