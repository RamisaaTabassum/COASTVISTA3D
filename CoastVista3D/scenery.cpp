
// CoastVista 3D - scenery.cpp
// Owner: Ramisa
// Day 3: Trees, Lamp Posts, Hotels, Resorts and Restaurants.

#include "scenery.h"

namespace
{
    // ---------------------------------------------------------
    // Basic cylinder along the Y axis
    // ---------------------------------------------------------

    void drawCylinderY(float bottomRadius,
        float topRadius,
        float height,
        int slices = 16)
    {
        const float PI = 3.14159265f;

        if (height <= 0.0f)
            return;

        // Cylinder sides
        glBegin(GL_QUAD_STRIP);

        for (int i = 0; i <= slices; ++i)
        {
            float angle = 2.0f * PI * i / slices;

            float x = cosf(angle);
            float z = sinf(angle);

            glNormal3f(x,
                (bottomRadius - topRadius) / height,
                z);

            glVertex3f(bottomRadius * x, 0.0f, bottomRadius * z);
            glVertex3f(topRadius * x, height, topRadius * z);
        }

        glEnd();

        // Bottom cap
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);

        for (int i = 0; i <= slices; ++i)
        {
            float angle = 2.0f * PI * i / slices;

            glVertex3f(
                bottomRadius * cosf(angle),
                0.0f,
                bottomRadius * sinf(angle)
            );
        }

        glEnd();

        // Top cap
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, height, 0.0f);

        for (int i = 0; i <= slices; ++i)
        {
            float angle = 2.0f * PI * i / slices;

            glVertex3f(
                topRadius * cosf(angle),
                height,
                topRadius * sinf(angle)
            );
        }

        glEnd();
    }


    // ---------------------------------------------------------
    // Basic 3D box
    // ---------------------------------------------------------

    void drawBox(
        float x0, float x1,
        float y0, float y1,
        float z0, float z1)
    {
        glBegin(GL_QUADS);

        // Front (+Z)
        glNormal3f(0, 0, 1);
        glVertex3f(x0, y0, z1);
        glVertex3f(x1, y0, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x0, y1, z1);

        // Back (-Z)
        glNormal3f(0, 0, -1);
        glVertex3f(x1, y0, z0);
        glVertex3f(x0, y0, z0);
        glVertex3f(x0, y1, z0);
        glVertex3f(x1, y1, z0);

        // Right (+X)
        glNormal3f(1, 0, 0);
        glVertex3f(x1, y0, z1);
        glVertex3f(x1, y0, z0);
        glVertex3f(x1, y1, z0);
        glVertex3f(x1, y1, z1);

        // Left (-X)
        glNormal3f(-1, 0, 0);
        glVertex3f(x0, y0, z0);
        glVertex3f(x0, y0, z1);
        glVertex3f(x0, y1, z1);
        glVertex3f(x0, y1, z0);

        // Top
        glNormal3f(0, 1, 0);
        glVertex3f(x0, y1, z0);
        glVertex3f(x0, y1, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x1, y1, z0);

        // Bottom
        glNormal3f(0, -1, 0);
        glVertex3f(x0, y0, z1);
        glVertex3f(x0, y0, z0);
        glVertex3f(x1, y0, z0);
        glVertex3f(x1, y0, z1);

        glEnd();
    }


    // ---------------------------------------------------------
    // Front-facing rectangle for building windows
    // ---------------------------------------------------------

    void drawFrontRect(
        float x0, float x1,
        float y0, float y1,
        float z,
        unsigned hex)
    {
        colorHex(hex);

        glNormal3f(0, 0, 1);

        glBegin(GL_QUADS);

        glVertex3f(x0, y0, z);
        glVertex3f(x1, y0, z);
        glVertex3f(x1, y1, z);
        glVertex3f(x0, y1, z);

        glEnd();
    }


    // ---------------------------------------------------------
    // Common building model
    // ---------------------------------------------------------

    void drawBuilding(
        float x,
        float z,
        float width,
        float depth,
        float height,
        int floors,
        int columns,
        unsigned wallColor,
        unsigned roofColor,
        unsigned windowColor)
    {
        float frontZ = z + depth * 0.5f;

        // Main building
        colorHex(wallColor);

        drawBox(
            x - width * 0.5f,
            x + width * 0.5f,
            0.0f,
            height,
            z - depth * 0.5f,
            frontZ
        );

        // Roof
        colorHex(roofColor);

        drawBox(
            x - width * 0.5f - 0.8f,
            x + width * 0.5f + 0.8f,
            height,
            height + 0.65f,
            z - depth * 0.5f - 0.5f,
            frontZ + 0.5f
        );

        // Windows
        float floorHeight = height / floors;
        float windowWidth = width / (columns * 2.3f);

        for (int floor = 0; floor < floors; ++floor)
        {
            float y0 = 0.9f + floor * floorHeight;
            float y1 = y0 + floorHeight * 0.48f;

            for (int col = 0; col < columns; ++col)
            {
                float windowX =
                    x - width * 0.38f
                    + col * (width * 0.76f / (columns - 1));

                // Dark window frame
                drawFrontRect(
                    windowX - windowWidth * 0.5f - 0.12f,
                    windowX + windowWidth * 0.5f + 0.12f,
                    y0 - 0.12f,
                    y1 + 0.12f,
                    frontZ + 0.025f,
                    0x403D38
                );

                // Glass
                drawFrontRect(
                    windowX - windowWidth * 0.5f,
                    windowX + windowWidth * 0.5f,
                    y0,
                    y1,
                    frontZ + 0.035f,
                    windowColor
                );

                // Window divider
                colorHex(0xD8D2C5);

                drawBox(
                    windowX - 0.045f,
                    windowX + 0.045f,
                    y0,
                    y1,
                    frontZ + 0.045f,
                    frontZ + 0.055f
                );
            }

            // Horizontal floor band
            drawFrontRect(
                x - width * 0.48f,
                x + width * 0.48f,
                y1 + 0.22f,
                y1 + 0.32f,
                frontZ + 0.06f,
                roofColor
            );
        }
    }

} // anonymous namespace



// =============================================================
// TREES
// =============================================================

void drawTree(float x, float z, float scale)
{
    glPushMatrix();

    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);

    // Tree trunk
    colorHex(0x79502E);

    drawCylinderY(0.24f, 0.14f, 4.6f, 12);

    // Main green canopy
    glPushMatrix();

    glTranslatef(0.0f, 5.2f, 0.0f);

    colorHex(0x286B35);
    glutSolidSphere(2.0, 12, 10);

    glTranslatef(-1.15f, 0.15f, 0.15f);
    colorHex(0x397E3C);
    glutSolidSphere(1.45, 10, 8);

    glTranslatef(2.3f, 0.0f, 0.0f);
    colorHex(0x458D45);
    glutSolidSphere(1.45, 10, 8);

    glTranslatef(-1.15f, 0.85f, -0.65f);
    colorHex(0x347A38);
    glutSolidSphere(1.5, 10, 8);

    glPopMatrix();

    glPopMatrix();
}


// Multiple trees in the forest area behind the buildings
void drawTrees()
{
    for (int row = 0; row < 3; ++row)
    {
        for (int i = 0; i < 18; ++i)
        {
            float x = -930.0f + i * 110.0f
                + (row % 2) * 45.0f;

            float z = -55.0f - row * 23.0f;

            float scale;

            switch ((i + row) % 4)
            {
            case 0: scale = 0.85f; break;
            case 1: scale = 1.00f; break;
            case 2: scale = 1.15f; break;
            default: scale = 0.95f; break;
            }

            if (x < ROAD_HALF_LENGTH)
                drawTree(x, z, scale);
        }
    }
}



// =============================================================
// LAMP POSTS
// =============================================================

void drawLampPost(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, SIDEWALK_H, z);

    // Lamp base
    colorHex(0x45484C);

    drawBox(
        -0.25f, 0.25f,
        0.0f, 0.35f,
        -0.25f, 0.25f
    );

    // Vertical pole
    colorHex(0x303438);

    drawCylinderY(0.14f, 0.10f, 6.7f, 12);

    // Horizontal lamp arm
    glPushMatrix();

    glTranslatef(0.0f, 6.35f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    colorHex(0x303438);

    drawCylinderY(0.07f, 0.05f, 0.85f, 10);

    glPopMatrix();

    // Lamp head
    colorHex(0x45484C);

    drawBox(
        -0.38f, 0.38f,
        6.1f, 6.42f,
        0.62f, 1.12f
    );

    // Warm lamp panel
    colorHex(0xF5D98B);

    drawBox(
        -0.29f, 0.29f,
        6.08f, 6.13f,
        0.70f, 1.05f
    );

    glPopMatrix();
}


// Lamps along the sea-side footpath
void drawLampPosts()
{
    for (float x = -900.0f; x <= 900.0f; x += 120.0f)
    {
        drawLampPost(x, 7.5f);
    }
}



// =============================================================
// HOTELS
// =============================================================

void drawHotel(float x, float z)
{
    drawBuilding(
        x, z,
        78.0f, 26.0f,
        17.0f,
        4, 6,
        0xD8C7A6,
        0x754336,
        0x376A85
    );

    // Hotel sign panel
    float frontZ = z + 13.0f;

    drawFrontRect(
        x - 18.0f,
        x + 18.0f,
        15.2f,
        16.2f,
        frontZ + 0.09f,
        0x8B3C35
    );
}



// =============================================================
// RESORTS
// =============================================================

void drawResort(float x, float z)
{
    drawBuilding(
        x, z,
        92.0f, 28.0f,
        12.0f,
        2, 7,
        0xD7C6A2,
        0x76503A,
        0x367B91
    );

    float frontZ = z + 14.0f;

    // Resort balcony / terrace band
    drawFrontRect(
        x - 43.0f,
        x + 43.0f,
        5.8f,
        6.1f,
        frontZ + 0.10f,
        0x80694E
    );
}



// =============================================================
// RESTAURANTS
// =============================================================

void drawRestaurant(float x, float z)
{
    drawBuilding(
        x, z,
        58.0f, 22.0f,
        7.0f,
        1, 5,
        0xE4D6B9,
        0x853F32,
        0x285B70
    );

    float frontZ = z + 11.0f;

    // Restaurant front canopy
    colorHex(0xA94E36);

    drawBox(
        x - 25.0f,
        x + 25.0f,
        4.8f,
        5.1f,
        frontZ + 0.15f,
        frontZ + 1.0f
    );

    // Restaurant entrance
    drawFrontRect(
        x - 2.0f,
        x + 2.0f,
        0.2f,
        3.7f,
        frontZ + 0.08f,
        0x263D49
    );
}



// =============================================================
// COMPLETE ROADSIDE BUILDINGS
// =============================================================

void drawRoadsideBuildings()
{
    drawHotel(-720.0f, -30.0f);

    drawResort(-470.0f, -30.0f);

    drawRestaurant(-220.0f, -30.0f);

    drawHotel(80.0f, -30.0f);

    drawResort(370.0f, -30.0f);

    drawRestaurant(650.0f, -30.0f);
}
