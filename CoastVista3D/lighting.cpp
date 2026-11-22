// CoastVista 3D - lighting.cpp
// Owner: Nafija Rahman
// Day 3: Day, Sunset and Night lighting presets.

#include "lighting.h"

namespace
{
    // Apply common OpenGL lighting and material settings.
    void applyPreset(
        const GLfloat sceneAmbient[4],
        const GLfloat lightAmbient[4],
        const GLfloat lightDiffuse[4],
        const GLfloat lightSpecular[4],
        const GLfloat lightPosition[4],
        const GLfloat materialSpecular[4],
        GLfloat shininess,
        const GLfloat clearColor[4]
    )
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(
            GL_FRONT_AND_BACK,
            GL_AMBIENT_AND_DIFFUSE
        );

        // Global scene lighting.
        glLightModelfv(
            GL_LIGHT_MODEL_AMBIENT,
            sceneAmbient
        );

        // Main directional light.
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        // Surface material settings.
        glMaterialfv(
            GL_FRONT_AND_BACK,
            GL_SPECULAR,
            materialSpecular
        );

        glMaterialf(
            GL_FRONT_AND_BACK,
            GL_SHININESS,
            shininess
        );

        glClearColor(
            clearColor[0],
            clearColor[1],
            clearColor[2],
            clearColor[3]
        );
    }
}

// ---------------------------------------------------------
// DAY LIGHTING
// ---------------------------------------------------------

void setDayLighting()
{
    const GLfloat sceneAmbient[4] =
        { 0.22f, 0.26f, 0.32f, 1.0f };

    const GLfloat lightAmbient[4] =
        { 0.08f, 0.08f, 0.08f, 1.0f };

    const GLfloat lightDiffuse[4] =
        { 0.80f, 0.76f, 0.68f, 1.0f };

    const GLfloat lightSpecular[4] =
        { 0.15f, 0.15f, 0.15f, 1.0f };

    const GLfloat lightPosition[4] =
        { SUN_DIR[0], SUN_DIR[1], SUN_DIR[2], 0.0f };

    const GLfloat materialSpecular[4] =
        { 0.18f, 0.18f, 0.18f, 1.0f };

    const GLfloat clearColor[4] =
        { 0.74f, 0.86f, 0.95f, 1.0f };

    applyPreset(
        sceneAmbient,
        lightAmbient,
        lightDiffuse,
        lightSpecular,
        lightPosition,
        materialSpecular,
        24.0f,
        clearColor
    );
}

// ---------------------------------------------------------
// SUNSET LIGHTING
// ---------------------------------------------------------

void setSunsetLighting()
{
    const GLfloat sceneAmbient[4] =
        { 0.28f, 0.13f, 0.10f, 1.0f };

    const GLfloat lightAmbient[4] =
        { 0.18f, 0.08f, 0.05f, 1.0f };

    const GLfloat lightDiffuse[4] =
        { 1.00f, 0.48f, 0.25f, 1.0f };

    const GLfloat lightSpecular[4] =
        { 0.24f, 0.12f, 0.08f, 1.0f };

    const GLfloat lightPosition[4] =
        { -0.70f, 0.32f, 0.45f, 0.0f };

    const GLfloat materialSpecular[4] =
        { 0.20f, 0.12f, 0.08f, 1.0f };

    const GLfloat clearColor[4] =
        { 0.95f, 0.42f, 0.22f, 1.0f };

    applyPreset(
        sceneAmbient,
        lightAmbient,
        lightDiffuse,
        lightSpecular,
        lightPosition,
        materialSpecular,
        20.0f,
        clearColor
    );
}

// ---------------------------------------------------------
// NIGHT LIGHTING
// ---------------------------------------------------------

void setNightLighting()
{
    const GLfloat sceneAmbient[4] =
        { 0.06f, 0.08f, 0.16f, 1.0f };

    const GLfloat lightAmbient[4] =
        { 0.04f, 0.06f, 0.12f, 1.0f };

    const GLfloat lightDiffuse[4] =
        { 0.22f, 0.30f, 0.52f, 1.0f };

    const GLfloat lightSpecular[4] =
        { 0.08f, 0.11f, 0.20f, 1.0f };

    const GLfloat lightPosition[4] =
        { 0.20f, 0.75f, 0.35f, 0.0f };

    const GLfloat materialSpecular[4] =
        { 0.10f, 0.14f, 0.24f, 1.0f };

    const GLfloat clearColor[4] =
        { 0.015f, 0.025f, 0.08f, 1.0f };

    applyPreset(
        sceneAmbient,
        lightAmbient,
        lightDiffuse,
        lightSpecular,
        lightPosition,
        materialSpecular,
        12.0f,
        clearColor
    );
}

// ---------------------------------------------------------
// INITIALIZATION AND MODE SELECTION
// ---------------------------------------------------------

void initializeSceneLighting()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    setDayLighting();
}

void applyLightingMode(LightingMode mode)
{
    switch (mode)
    {
    case LightingMode::Day:
        setDayLighting();
        break;

    case LightingMode::Sunset:
        setSunsetLighting();
        break;

    case LightingMode::Night:
        setNightLighting();
        break;

    default:
        setDayLighting();
        break;
    }
}