// CoastVista 3D - road.cpp   (owner: Ramisa)
// Day 2: road, lane markings, zebra crossings, footpaths, kerbs,
// beach (dry / wet sand, foam), land ground.
//
// How it stays realistic without textures:
//   - small, repeatable colour differences (asphalt patches, paving tiles, sand, grass)
//   - worn paint, tyre tracks, repair patches
//   - dry sand -> wet sand -> foam line at the shore
// All of it is static, so it is recorded ONCE into OpenGL display lists and replayed every frame.

#include "scene.h"

namespace {

    // ---- palette ----------------------------------------------------------------
    const unsigned ASPHALT = 0x26262A;
    const unsigned PAINT = 0xF2F2EE;
    const unsigned FOOTPATH = 0x9E9C94;
    const unsigned KERB_TOP = 0xB8B6AE;
    const unsigned KERB_FACE = 0x85837C;
    const unsigned VERGE = 0x8E7F5F;
    const unsigned GRASS_A = 0x3A6B2A;
    const unsigned GRASS_B = 0x4C7B31;
    const unsigned GRASS_C = 0x2F5A24;
    const unsigned FOAM = 0xF4F7F5;
    const unsigned FOAM_MID = 0x9AD3D4;
    const unsigned SEA_EDGE = 0x1F8FA0;

    const unsigned SAND_ROW[6] = {
        0xE4D5AF, 0xDCCB9F, 0xD0BC8F,
        0xBBA77C, 0x94835F, 0x94835F
    };

    // ---- helpers ----------------------------------------------------------------

    float rnd(int a, int b = 0, int c = 0)
    {
        unsigned int h = (unsigned)a * 374761393u
            + (unsigned)b * 668265263u
            + (unsigned)c * 2147483647u
            + 1013904223u;

        h = (h ^ (h >> 13)) * 1274126177u;
        h ^= (h >> 16);

        return (h & 0xFFFFu) / 65535.0f;
    }

    inline float cl(float v)
    {
        return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
    }

    void toRGB(unsigned hex, float out[3])
    {
        out[0] = ((hex >> 16) & 255) / 255.0f;
        out[1] = ((hex >> 8) & 255) / 255.0f;
        out[2] = (hex & 255) / 255.0f;
    }

    void colorShade(unsigned hex, float k)
    {
        float c[3];
        toRGB(hex, c);

        glColor3f(
            cl(c[0] * k),
            cl(c[1] * k),
            cl(c[2] * k)
        );
    }

    void colorMix(unsigned a, unsigned b, float t, float k)
    {
        float ca[3], cb[3];

        toRGB(a, ca);
        toRGB(b, cb);

        glColor3f(
            cl((ca[0] + (cb[0] - ca[0]) * t) * k),
            cl((ca[1] + (cb[1] - ca[1]) * t) * k),
            cl((ca[2] + (cb[2] - ca[2]) * t) * k)
        );
    }

    // Zebra crossings along the road.
    const float CROSSING_X[5] = {
        -600.0f, -300.0f, 40.0f, 400.0f, 750.0f
    };

    const float CROSSING_LEN = 3.6f;

    bool nearCrossing(float x0, float x1)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (x1 > CROSSING_X[i] - 0.5f &&
                x0 < CROSSING_X[i] + CROSSING_LEN + 0.5f)
                return true;
        }

        return false;
    }

    // ---- road -------------------------------------------------------------------

    void buildRoad()
    {
        const float L = ROAD_HALF_LENGTH;
        const float W = ROAD_HALF_WIDTH;
        const float SW = W + SIDEWALK_W;
        const float H = SIDEWALK_H;

        // 1. Asphalt: 25 m patches.
        const float seg = 25.0f;
        const int nSeg = (int)(2.0f * L / seg);

        for (int lane = 0; lane < 2; ++lane)
        {
            float z0 = (lane == 0) ? -W : 0.0f;
            float z1 = (lane == 0) ? 0.0f : W;

            for (int i = 0; i < nSeg; ++i)
            {
                float x0 = -L + i * seg;

                colorShade(ASPHALT,
                    0.93f + 0.12f * rnd(i, lane, 1));

                quadXZ(x0, x0 + seg, z0, z1, 0.0f);
            }
        }

        // 2. Footpaths: 1.5 m paving tiles.
        const float tile = 1.5f;
        const int nTile = (int)(2.0f * L / tile);

        for (int side = 0; side < 2; ++side)
        {
            for (int row = 0; row < 2; ++row)
            {
                float zNear = (side == 0)
                    ? (W + row * 1.5f)
                    : -(W + (row + 1) * 1.5f);

                for (int i = 0; i < nTile; ++i)
                {
                    float x0 = -L + i * tile;

                    colorShade(FOOTPATH,
                        0.92f + 0.15f * rnd(i, row, 10 + side));

                    quadXZ(x0, x0 + tile,
                        zNear, zNear + 1.5f, H);
                }
            }
        }

        // 3. Kerb faces and footpath edges.
        colorShade(KERB_FACE, 1.0f);

        wallZ(-L, L, 0.0f, H, W, -1.0f);
        wallZ(-L, L, 0.0f, H, -W, +1.0f);
        wallZ(-L, L, SAND_Y0, H, SW, +1.0f);
        wallZ(-L, L, 0.0f, H, -SW, -1.0f);

        // Surface details.
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(-1.0f, -1.0f);

        // Kerb tops.
        colorShade(KERB_TOP, 1.0f);

        quadXZ(-L, L, W, W + 0.25f, H + 0.01f);
        quadXZ(-L, L, -W - 0.25f, -W, H + 0.01f);
        quadXZ(-L, L, SW - 0.25f, SW, H + 0.01f);
        quadXZ(-L, L, -SW, -SW + 0.25f, H + 0.01f);

        // Repair patches on asphalt.
        for (int lane = 0; lane < 2; ++lane)
        {
            for (int i = 0; i < nSeg; ++i)
            {
                if (rnd(i, lane, 2) < 0.88f)
                    continue;

                float x0 = -L + i * seg
                    + 2.0f + 6.0f * rnd(i, lane, 3);

                float z0 = (lane == 0 ? -W + 0.8f : 0.8f)
                    + 2.0f * rnd(i, lane, 4);

                colorShade(ASPHALT,
                    (rnd(i, lane, 5) < 0.5f) ? 0.78f : 1.18f);

                quadXZ(x0,
                    x0 + 4.0f + 5.0f * rnd(i, lane, 6),
                    z0,
                    z0 + 1.2f + 2.0f * rnd(i, lane, 7),
                    0.012f);
            }
        }

        // Tyre tracks.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const float laneCentre[2] = { -2.65f, 2.65f };

        for (int lane = 0; lane < 2; ++lane)
        {
            for (int w = 0; w < 2; ++w)
            {
                float zc = laneCentre[lane]
                    + (w == 0 ? -0.85f : 0.85f);

                for (int i = 0; i < (int)(2.0f * L / 50.0f); ++i)
                {
                    float x0 = -L + i * 50.0f;

                    glColor4f(0.0f, 0.0f, 0.0f,
                        0.06f + 0.07f * rnd(i, lane * 2 + w, 8));

                    quadXZ(x0, x0 + 50.0f,
                        zc - 0.25f, zc + 0.25f, 0.016f);
                }
            }
        }

        glDisable(GL_BLEND);

        // 4. Road paint.
        glPolygonOffset(-2.0f, -2.0f);

        // Dashed centre line: 3 m paint, 6 m gap.
        for (float x = -L, i = 0; x < L; x += 9.0f, i += 1.0f)
        {
            if (nearCrossing(x, x + 3.0f))
                continue;

            colorShade(PAINT,
                0.86f + 0.12f * rnd((int)i, 0, 20));

            quadXZ(x, x + 3.0f, -0.08f, 0.08f, 0.03f);
        }

        // Solid edge lines.
        for (int s = 0; s < 2; ++s)
        {
            float z = (s == 0 ? -1.0f : 1.0f)
                * (W - 0.7f);

            for (int i = 0; i < nSeg; ++i)
            {
                float x0 = -L + i * seg;

                colorShade(PAINT,
                    0.84f + 0.14f * rnd(i, s, 21));

                quadXZ(x0, x0 + seg,
                    z - 0.07f, z + 0.07f, 0.03f);
            }
        }

        // 5. Five zebra crossings.
        for (int c = 0; c < 5; ++c)
        {
            for (int s = 0; s < 9; ++s)
            {
                float z0 = -4.275f + s * 1.0f;

                colorShade(PAINT,
                    0.90f + 0.08f * rnd(c, s, 22));

                quadXZ(
                    CROSSING_X[c],
                    CROSSING_X[c] + CROSSING_LEN,
                    z0,
                    z0 + 0.55f,
                    0.034f
                );
            }
        }

        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    // ---- beach and land ---------------------------------------------------------

    void buildBeach()
    {
        const float L = WORLD_HALF_LENGTH;
        const float step = 50.0f;
        const int cols = (int)(2.0f * L / step);

        const float slope =
            (SAND_Y1 - SAND_Y0) / (SAND_Z1 - SAND_Z0);

        const float shore =
            SAND_Z0 + (SEA_Y - SAND_Y0) / slope;

        // 1. Dry sand to wet sand.
        const float rowZ[6] = {
            SAND_Z0, 16.0f, 25.0f, 30.0f, shore, SAND_Z1
        };

        glNormal3f(0.0f, 1.0f, -slope);
        glBegin(GL_QUADS);

        for (int c = 0; c < cols; ++c)
        {
            float xa = -L + c * step;
            float xb = xa + step;

            for (int r = 0; r < 5; ++r)
            {
                const float xs[4] = { xa, xb, xb, xa };
                const int cs[4] = { c, c + 1, c + 1, c };
                const int rs[4] = { r, r, r + 1, r + 1 };

                for (int v = 0; v < 4; ++v)
                {
                    float z = rowZ[rs[v]];

                    colorShade(SAND_ROW[rs[v]],
                        0.955f + 0.09f * rnd(cs[v], rs[v], 3));

                    glVertex3f(xs[v],
                        SAND_Y0 + (z - SAND_Z0) * slope,
                        z);
                }
            }
        }

        glEnd();

        // 2. Shoreline foam.
        const float fy = SEA_Y + 0.03f;
        const float z0 = shore - 0.03f / (-slope);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);

        for (int c = 0; c < cols; ++c)
        {
            float xa = -L + c * step;
            float xb = xa + step;

            float za1 = shore + 0.5f + 0.5f * rnd(c, 1, 30);
            float zb1 = shore + 0.5f + 0.5f * rnd(c + 1, 1, 30);

            float za2 = shore + 1.6f + 0.9f * rnd(c, 2, 30);
            float zb2 = shore + 1.6f + 0.9f * rnd(c + 1, 2, 30);

            float za3 = shore + 3.4f + 1.0f * rnd(c, 3, 30);
            float zb3 = shore + 3.4f + 1.0f * rnd(c + 1, 3, 30);

            // Sand edge to bright foam.
            colorShade(FOAM, 1.0f);
            glVertex3f(xa, fy, z0);
            glVertex3f(xb, fy, z0);

            colorShade(FOAM, 1.0f);
            glVertex3f(xb, fy, zb1);
            glVertex3f(xa, fy, za1);

            // Bright foam to aqua.
            colorShade(FOAM, 1.0f);
            glVertex3f(xa, fy, za1);
            glVertex3f(xb, fy, zb1);

            colorShade(FOAM_MID, 1.0f);
            glVertex3f(xb, fy, zb2);
            glVertex3f(xa, fy, za2);

            // Aqua to sea colour.
            colorShade(FOAM_MID, 1.0f);
            glVertex3f(xa, fy, za2);
            glVertex3f(xb, fy, zb2);

            colorShade(SEA_EDGE, 1.0f);
            glVertex3f(xb, fy, zb3);
            glVertex3f(xa, fy, za3);
        }

        glEnd();

        // 3. Land side: dusty verge and patchy grass.
        const float rowL[7] = {
            -(ROAD_HALF_WIDTH + SIDEWALK_W),
            -11.0f, -17.0f, -40.0f,
            -80.0f, -130.0f, -180.0f
        };

        glNormal3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);

        for (int c = 0; c < cols; ++c)
        {
            float xa = -L + c * step;
            float xb = xa + step;

            for (int r = 0; r < 6; ++r)
            {
                const float xs[4] = { xa, xb, xb, xa };
                const int cs[4] = { c, c + 1, c + 1, c };
                const int rs[4] = { r, r, r + 1, r + 1 };

                for (int v = 0; v < 4; ++v)
                {
                    int rr = rs[v];

                    if (rr <= 1)
                    {
                        colorShade(VERGE,
                            0.92f + 0.12f * rnd(cs[v], rr, 40));
                    }
                    else
                    {
                        colorMix(GRASS_C, GRASS_B,
                            rnd(cs[v], rr, 41),
                            0.94f + 0.12f * rnd(cs[v], rr, 42));
                    }

                    glVertex3f(xs[v], 0.0f, rowL[rr]);
                }
            }
        }

        glEnd();

        // Far land behind the hills.
        colorShade(GRASS_A, 1.0f);
        quadXZ(-L, L, WORLD_Z_LAND, rowL[6], 0.0f);
    }

    // ---- display list cache -----------------------------------------------------

    template <class F>
    void drawCached(GLuint& list, F build)
    {
        if (!list)
        {
            list = glGenLists(1);
            glNewList(list, GL_COMPILE);
            build();
            glEndList();
        }

        glCallList(list);
    }

} // namespace

// Public scene functions.
void drawRoad()
{
    static GLuint list = 0;
    drawCached(list, buildRoad);
}

void drawBeach()
{
    static GLuint list = 0;
    drawCached(list, buildBeach);
}