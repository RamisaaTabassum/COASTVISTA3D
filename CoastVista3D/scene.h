// CoastVista 3D - scene.h
// Scene modules + tiny drawing helpers shared by every module.

#pragma once

#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/freeglut.h>

#include "config.h"

// ---- Scene modules (each one lives in its own .cpp file) ------------------

void drawRoad();             // road.cpp (Ramisa)
void drawBeach();            // road.cpp (Ramisa)
void drawSea(float timeSec); // sea.cpp (Nafija)
void drawMountains();        // sea.cpp (Nafija)

// ---- Helpers ---------------------------------------------------------------

// glColor from a web colour like 0xD8C39A.
inline void colorHex(unsigned hex)
{
    glColor3f(
        ((hex >> 16) & 255) / 255.0f,
        ((hex >> 8) & 255) / 255.0f,
        (hex & 255) / 255.0f
    );
}

// Flat horizontal quad (facing up).
inline void quadXZ(
    float x0, float x1,
    float z0, float z1,
    float y
)
{
    glNormal3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);

    glVertex3f(x0, y, z1);
    glVertex3f(x1, y, z1);
    glVertex3f(x1, y, z0);
    glVertex3f(x0, y, z0);

    glEnd();
}

// Vertical wall in the plane z = const.
inline void wallZ(
    float x0, float x1,
    float y0, float y1,
    float z, float nz
)
{
    glNormal3f(0.0f, 0.0f, nz);

    glBegin(GL_QUADS);

    if (nz > 0.0f)
    {
        glVertex3f(x0, y0, z);
        glVertex3f(x1, y0, z);
        glVertex3f(x1, y1, z);
        glVertex3f(x0, y1, z);
    }
    else
    {
        glVertex3f(x1, y0, z);
        glVertex3f(x0, y0, z);
        glVertex3f(x0, y1, z);
        glVertex3f(x1, y1, z);
    }

    glEnd();
}