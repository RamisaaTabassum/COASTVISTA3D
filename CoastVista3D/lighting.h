// CoastVista 3D - lighting.h
// Owner: Nafija Rahman
// Day 3: Day, Sunset and Night lighting presets.

#pragma once

#include "scene.h"

// Available lighting modes.
enum class LightingMode
{
    Day,
    Sunset,
    Night
};

// Initialize OpenGL lighting and material settings.
void initializeSceneLighting();

// Apply the selected lighting preset.
void applyLightingMode(LightingMode mode);

// Individual lighting presets.
void setDayLighting();
void setSunsetLighting();
void setNightLighting();