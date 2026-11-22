#pragma once

// CoastVista 3D - config.h
// Shared constants for the whole team. Scene layout numbers live HERE so that
// Ramisa's and Nafija's modules always agree on where things are.
// If you change a number, tell your teammate (it is a shared file).

// ---------------------------------------------------------------- window ---
#define WINDOW_TITLE "CoastVista 3D"
#define USE_MSAA 1

static const int WINDOW_W = 1280;
static const int WINDOW_H = 720;

// ------------------------------------------------------------ projection ---
static const float FOV_Y = 60.0f;
static const float Z_NEAR = 0.5f;
static const float Z_FAR = 1500.0f;

// ------------------------------------------------------------ scene layout ---
// Units: 1 unit = 1 metre.
// +X = along the road, +Y = up, +Z = towards the SEA (land is -Z).

static const float ROAD_HALF_LENGTH = 1000.0f;
static const float WORLD_HALF_LENGTH = ROAD_HALF_LENGTH + 100.0f;

static const float ROAD_HALF_WIDTH = 6.0f;
static const float SIDEWALK_W = 3.0f;
static const float SIDEWALK_H = 0.15f;

static const float SAND_Z0 = ROAD_HALF_WIDTH + SIDEWALK_W;
static const float SAND_Z1 = 45.0f;
static const float SAND_Y0 = -0.2f;
static const float SAND_Y1 = -1.2f;

static const float SEA_Y = -0.9f;
static const float SEA_Z0 = 30.0f;
static const float WORLD_Z_SEA = 1400.0f;
static const float WORLD_Z_LAND = -1200.0f;

// Land side (negative Z)
static const float BUILDING_Z_NEAR = -14.0f;
static const float BUILDING_Z_FAR = -45.0f;
static const float FOREST_Z_FAR = -110.0f;
static const float HILL_Z_NEAR = -180.0f;
static const float HILL_Z_FAR = -300.0f;

// --------------------------------------------------------- default camera ---
static const float CAM_START_X = -60.0f;
static const float CAM_START_Y = 1.7f;
static const float CAM_START_Z = 7.5f;
static const float CAM_START_YAW = 4.0f;
static const float CAM_START_PITCH = 1.0f;

// ---------------------------------------------------------- sun and sky ---
static const float SUN_DIR[3] = { 0.45f, 0.85f, 0.30f };
static const float SKY_TOP[3] = { 0.18f, 0.40f, 0.78f };
static const float SKY_HORIZON[3] = { 0.74f, 0.86f, 0.95f };