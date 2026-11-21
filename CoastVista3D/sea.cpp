// CoastVista 3D - sea.cpp   (owner: Nafija)
// Day 1: simple layout preview. Day 2: sea with a base wave shape + proper hills.

#include "scene.h"

// Sea surface. Day 1 = flat plane with a shallow -> deep colour gradient.
// timeSec is not used yet; it will drive the waves.

void drawSea(float timeSec)
{
    (void)timeSec;
    const float L = WORLD_HALF_LENGTH;

    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);

    colorHex(0x1F8FA0);  // shallow, turquoise
    glVertex3f(-L, SEA_Y, SEA_Z0);
    glVertex3f(L, SEA_Y, SEA_Z0);

    colorHex(0x0B3A6B);  // deep blue far away
    glVertex3f(L, SEA_Y, WORLD_Z_SEA);
    glVertex3f(-L, SEA_Y, WORLD_Z_SEA);

    glEnd();
}

// Background hills: a ridge that rises from HILL_Z_NEAR to HILL_Z_FAR.

void drawMountains()
{
    const float L = WORLD_HALF_LENGTH;
    const float step = 20.0f;
    const float dz = HILL_Z_NEAR - HILL_Z_FAR;

    glBegin(GL_QUAD_STRIP);

    for (float x = -L; x <= L; x += step)
    {
        float h = 55.0f + 28.0f * sinf(0.0045f * x)
            + 16.0f * sinf(0.0130f * x + 1.3f)
            + 9.0f * sinf(0.0310f * x + 0.4f);

        glNormal3f(0.0f, dz, h);

        colorHex(0x3F6B3A);  // green at the foot
        glVertex3f(x, 0.0f, HILL_Z_NEAR);

        colorHex(0x6E8C86);  // blue-grey haze at the top
        glVertex3f(x, h, HILL_Z_FAR);
    }

    glEnd();
}