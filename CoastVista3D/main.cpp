// CoastVista 3D - main.cpp
// Day 1: Window, render loop, camera and projection.
// Day 3: Integrated day, sunset and night lighting.
//
// Coordinates: +X along road, +Y up, +Z towards sea.

#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/freeglut.h>

#include "config.h"
#include "scene.h"
#include "scenery.h"
#include "lighting.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

namespace
{
    struct Camera
    {
        float x, y, z;
        float yaw;
        float pitch;
    };

    Camera g_cam = {
        CAM_START_X,
        CAM_START_Y,
        CAM_START_Z,
        CAM_START_YAW,
        CAM_START_PITCH
    };

    int g_lastMs = 0;
    float g_time = 0.0f;
    int g_frames = 0;
    int g_fpsClockMs = 0;

    // Current lighting mode.
    LightingMode g_lightingMode = LightingMode::Day;

    inline float deg2rad(float d)
    {
        return d * 3.14159265f / 180.0f;
    }

    inline float mixf(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    inline float clamp01(float v)
    {
        return v < 0.0f ? 0.0f :
               (v > 1.0f ? 1.0f : v);
    }

    // ---------------------------------------------------------
    // SKY COLOURS
    // ---------------------------------------------------------

    void getSkyColors(GLfloat horizon[3], GLfloat top[3])
    {
        static const GLfloat sunsetHorizon[3] =
            { 0.98f, 0.42f, 0.20f };

        static const GLfloat sunsetTop[3] =
            { 0.28f, 0.12f, 0.32f };

        static const GLfloat nightHorizon[3] =
            { 0.015f, 0.025f, 0.08f };

        static const GLfloat nightTop[3] =
            { 0.005f, 0.008f, 0.03f };

        const GLfloat* h = SKY_HORIZON;
        const GLfloat* t = SKY_TOP;

        if (g_lightingMode == LightingMode::Sunset)
        {
            h = sunsetHorizon;
            t = sunsetTop;
        }
        else if (g_lightingMode == LightingMode::Night)
        {
            h = nightHorizon;
            t = nightTop;
        }

        for (int i = 0; i < 3; ++i)
        {
            horizon[i] = h[i];
            top[i] = t[i];
        }
    }

    // ---------------------------------------------------------
    // SKY RENDERING
    // ---------------------------------------------------------

    void drawSky()
    {
        GLfloat horizon[3];
        GLfloat top[3];

        getSkyColors(horizon, top);

        float t =
            tanf(deg2rad(g_cam.pitch)) /
            tanf(deg2rad(FOV_Y * 0.5f));

        float hy = clamp01(0.5f - 0.5f * t);
        float k = clamp01((1.0f - hy) / 0.65f);

        GLfloat skyTop[3];

        for (int i = 0; i < 3; ++i)
        {
            skyTop[i] = mixf(horizon[i], top[i], k);
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glOrtho(
            0.0, 1.0,
            0.0, 1.0,
            -1.0, 1.0
        );

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glBegin(GL_QUADS);

        glColor3fv(horizon);

        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(1.0f, hy);
        glVertex2f(0.0f, hy);

        glColor3fv(horizon);

        glVertex2f(0.0f, hy);
        glVertex2f(1.0f, hy);

        glColor3fv(skyTop);

        glVertex2f(1.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);

        glEnd();

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }

    // ---------------------------------------------------------
    // DISPLAY
    // ---------------------------------------------------------

    void display()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawSky();

        float yaw = deg2rad(g_cam.yaw);
        float pit = deg2rad(g_cam.pitch);

        float dx = cosf(yaw) * cosf(pit);
        float dy = sinf(pit);
        float dz = sinf(yaw) * cosf(pit);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(
            g_cam.x,
            g_cam.y,
            g_cam.z,

            g_cam.x + dx,
            g_cam.y + dy,
            g_cam.z + dz,

            0.0,
            1.0,
            0.0
        );

        // Apply lighting after setting the camera.
        // This ensures the light direction is correct.

        applyLightingMode(g_lightingMode);

        // Existing scene modules.

        drawBeach();
        drawRoad();
        drawSea(g_time);
        drawMountains();

        // Day 3 - Ramisa's Scenery.

        drawTrees();
        drawLampPosts();
        drawRoadsideBuildings();

        glutSwapBuffers();
    }

    // ---------------------------------------------------------
    // WINDOW RESIZE
    // ---------------------------------------------------------

    void reshape(int w, int h)
    {
        if (h == 0)
            h = 1;

        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(
            FOV_Y,
            (double)w / (double)h,
            Z_NEAR,
            Z_FAR
        );

        glMatrixMode(GL_MODELVIEW);
    }

    // ---------------------------------------------------------
    // ANIMATION TIMER
    // ---------------------------------------------------------

    void tick(int)
    {
        int now = glutGet(GLUT_ELAPSED_TIME);

        float dt = (now - g_lastMs) / 1000.0f;

        g_lastMs = now;
        g_time += dt;

        ++g_frames;

        if (now - g_fpsClockMs >= 1000)
        {
            char title[96];

            std::snprintf(
                title,
                sizeof(title),
                "%s | %d FPS",
                WINDOW_TITLE,
                g_frames
            );

            glutSetWindowTitle(title);

            g_frames = 0;
            g_fpsClockMs = now;
        }

        glutPostRedisplay();
        glutTimerFunc(16, tick, 0);
    }

    // ---------------------------------------------------------
    // KEYBOARD CONTROLS
    // ---------------------------------------------------------

    void keyboard(unsigned char key, int, int)
    {
        if (key == 27)
        {
            glutLeaveMainLoop();
            return;
        }

        switch (key)
        {
        case '1':
            g_lightingMode = LightingMode::Day;
            break;

        case '2':
            g_lightingMode = LightingMode::Sunset;
            break;

        case '3':
            g_lightingMode = LightingMode::Night;
            break;

        default:
            break;
        }

        glutPostRedisplay();
    }

    // ---------------------------------------------------------
    // OPENGL INITIALIZATION
    // ---------------------------------------------------------

    void initGL()
    {
        glClearColor(
            SKY_HORIZON[0],
            SKY_HORIZON[1],
            SKY_HORIZON[2],
            1.0f
        );

        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glEnable(GL_MULTISAMPLE);

        // Initialize the new lighting module.

        initializeSceneLighting();
    }

}

// =============================================================
// MAIN FUNCTION
// =============================================================

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    unsigned int mode =
        GLUT_DOUBLE |
        GLUT_RGBA |
        GLUT_DEPTH;

#if USE_MSAA
    mode |= GLUT_MULTISAMPLE;
    glutSetOption(GLUT_MULTISAMPLE, 4);
#endif

    glutInitDisplayMode(mode);

    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutInitWindowPosition(100, 60);

    glutCreateWindow(WINDOW_TITLE);

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    g_lastMs = g_fpsClockMs =
        glutGet(GLUT_ELAPSED_TIME);

    glutTimerFunc(0, tick, 0);

    glutMainLoop();

    return 0;
}