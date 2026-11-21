// CoastVista 3D - road.cpp   (owner: Ramisa)
// Day 1: simple layout preview. Day 2: detailed road, lane markings, beach.
#include "scene.h"

// Asphalt, lane markings, footpaths and kerbs.
void drawRoad()
{
    const float L = ROAD_HALF_LENGTH;
    const float W = ROAD_HALF_WIDTH;
    const float SW = W + SIDEWALK_W;
    const float H = SIDEWALK_H;

    // asphalt
    colorHex(0x26262A);
    quadXZ(-L, L, -W, W, 0.0f);

    // footpaths (top) + kerb faces
    colorHex(0x9E9C94);
    quadXZ(-L, L, W, SW, H);
    quadXZ(-L, L, -SW, -W, H);
    colorHex(0x85837C);
    wallZ(-L, L, 0.0f, H, W, -1.0f);
    wallZ(-L, L, 0.0f, H, -W, +1.0f);
    wallZ(-L, L, SAND_Y0, H, SW, +1.0f);
    wallZ(-L, L, 0.0f, H, -SW, -1.0f);

    // lane markings
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-2.0f, -2.0f);
    colorHex(0xF2F2EE);

    for (float x = -L; x < L; x += 9.0f)
        quadXZ(x, x + 3.0f, -0.08f, 0.08f, 0.03f);

    quadXZ(-L, L, W - 0.7f - 0.07f, W - 0.7f + 0.07f, 0.03f);
    quadXZ(-L, L, -W + 0.7f - 0.07f, -W + 0.7f + 0.07f, 0.03f);

    glDisable(GL_POLYGON_OFFSET_FILL);
}

// Sand slope on the sea side, plain ground on the land side.
void drawBeach()
{
    const float L = WORLD_HALF_LENGTH;

    // land side
    colorHex(0x3A6B2A);
    quadXZ(-L, L, WORLD_Z_LAND,
        -(ROAD_HALF_WIDTH + SIDEWALK_W), 0.0f);

    // sand slope
    const float slope = (SAND_Y1 - SAND_Y0) / (SAND_Z1 - SAND_Z0);

    colorHex(0xD8C39A);
    glNormal3f(0.0f, 1.0f, -slope);

    glBegin(GL_QUADS);
    glVertex3f(-L, SAND_Y0, SAND_Z0);
    glVertex3f(L, SAND_Y0, SAND_Z0);
    glVertex3f(L, SAND_Y1, SAND_Z1);
    glVertex3f(-L, SAND_Y1, SAND_Z1);
    glEnd();
}