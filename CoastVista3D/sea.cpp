// COASTVISTA 3D - sea.cpp
// Owner: Nafija Rahman
// Day 2: Base wave-shaped sea surface and hills/mountain background.
// Wave animation is reserved for the later animation stage.

#include "scene.h"

namespace
{
    // Keep all new helpers private to this file.

    float clamp01(float value)
    {
        if (value < 0.0f) return 0.0f;
        if (value > 1.0f) return 1.0f;
        return value;
    }

    void blendColor(unsigned colorA, unsigned colorB,
                    float t, float shade = 1.0f)
    {
        t = clamp01(t);

        float ar = ((colorA >> 16) & 255) / 255.0f;
        float ag = ((colorA >> 8) & 255) / 255.0f;
        float ab = (colorA & 255) / 255.0f;

        float br = ((colorB >> 16) & 255) / 255.0f;
        float bg = ((colorB >> 8) & 255) / 255.0f;
        float bb = (colorB & 255) / 255.0f;

        glColor3f(
            (ar + (br - ar) * t) * shade,
            (ag + (bg - ag) * t) * shade,
            (ab + (bb - ab) * t) * shade
        );
    }

    // ---------------------------------------------------------
    // SEA: static base wave shape
    // ---------------------------------------------------------

    float seaHeight(float x, float z)
    {
        // Gradually increase wave height away from the shoreline.
        float fade = clamp01((z - SEA_Z0) / 90.0f);

        float wave1 =
            0.10f * sinf(x * 0.012f + z * 0.030f);

        float wave2 =
            0.055f * sinf(x * 0.022f - z * 0.018f + 1.2f);

        float wave3 =
            0.025f * sinf(x * 0.035f + z * 0.012f + 0.7f);

        return SEA_Y + fade * (wave1 + wave2 + wave3);
    }

    void seaNormal(float x, float z)
    {
        const float e = 1.0f;

        float dx =
            (seaHeight(x + e, z) -
             seaHeight(x - e, z)) / (2.0f * e);

        float dz =
            (seaHeight(x, z + e) -
             seaHeight(x, z - e)) / (2.0f * e);

        float nx = -dx;
        float ny = 1.0f;
        float nz = -dz;

        float length = sqrtf(nx * nx + ny * ny + nz * nz);

        glNormal3f(
            nx / length,
            ny / length,
            nz / length
        );
    }

    void seaColor(float z)
    {
        float depth = (z - SEA_Z0) /
                      (WORLD_Z_SEA - SEA_Z0);

        // Shallow turquoise -> deep ocean blue.
        blendColor(0x1F8FA0, 0x0B3A6B, depth);
    }

    // ---------------------------------------------------------
    // HILLS AND MOUNTAINS
    // ---------------------------------------------------------

    float hillHeight(float x, float z)
    {
        float depth = clamp01(
            (HILL_Z_NEAR - z) /
            (HILL_Z_NEAR - HILL_Z_FAR)
        );

        // Natural variation along the mountain ridge.
        float ridge =
            70.0f
            + 15.0f * sinf(0.0045f * x)
            + 10.0f * sinf(0.0130f * x + 1.3f)
            + 5.0f  * sinf(0.0310f * x + 0.4f);

        return depth * ridge;
    }

    void hillNormal(float x, float z)
    {
        const float e = 1.0f;

        float dx =
            (hillHeight(x + e, z) -
             hillHeight(x - e, z)) / (2.0f * e);

        float dz =
            (hillHeight(x, z + e) -
             hillHeight(x, z - e)) / (2.0f * e);

        float nx = -dx;
        float ny = 1.0f;
        float nz = -dz;

        float length = sqrtf(nx * nx + ny * ny + nz * nz);

        glNormal3f(
            nx / length,
            ny / length,
            nz / length
        );
    }

    void hillColor(float x, float z)
    {
        float depth = clamp01(
            (HILL_Z_NEAR - z) /
            (HILL_Z_NEAR - HILL_Z_FAR)
        );

        float shade =
            0.92f + 0.08f *
            (0.5f + 0.5f * sinf(x * 0.01f));

        // Green hill base -> distant blue-grey haze.
        blendColor(0x3F6B3A, 0x6E8C86, depth, shade);
    }
}

// =============================================================
// SEA SURFACE
// =============================================================

void drawSea(float timeSec)
{
    // Day 2 uses a static wave shape.
    // timeSec is reserved for the later animation stage.
    (void)timeSec;

    const float L = WORLD_HALF_LENGTH;

    const float xStep = 20.0f;
    const float zStep = 25.0f;

    for (float z0 = SEA_Z0;
         z0 < WORLD_Z_SEA;
         z0 += zStep)
    {
        float z1 = z0 + zStep;

        if (z1 > WORLD_Z_SEA)
            z1 = WORLD_Z_SEA;

        glBegin(GL_QUAD_STRIP);

        for (float x = -L; x <= L; x += xStep)
        {
            // Near side of the sea.
            seaColor(z0);
            seaNormal(x, z0);

            glVertex3f(
                x,
                seaHeight(x, z0),
                z0
            );

            // Far side of the sea.
            seaColor(z1);
            seaNormal(x, z1);

            glVertex3f(
                x,
                seaHeight(x, z1),
                z1
            );
        }

        glEnd();
    }
}

// =============================================================
// HILLS AND MOUNTAIN BACKGROUND
// =============================================================

void drawMountains()
{
    const float L = WORLD_HALF_LENGTH;

    const float xStep = 20.0f;
    const float zStep = 10.0f;

    for (float z0 = HILL_Z_NEAR;
         z0 > HILL_Z_FAR;
         z0 -= zStep)
    {
        float z1 = z0 - zStep;

        if (z1 < HILL_Z_FAR)
            z1 = HILL_Z_FAR;

        glBegin(GL_QUAD_STRIP);

        for (float x = -L; x <= L; x += xStep)
        {
            // Far mountain edge.
            hillColor(x, z1);
            hillNormal(x, z1);

            glVertex3f(
                x,
                hillHeight(x, z1),
                z1
            );

            // Near hill edge.
            hillColor(x, z0);
            hillNormal(x, z0);

            glVertex3f(
                x,
                hillHeight(x, z0),
                z0
            );
        }

        glEnd();
    }
}