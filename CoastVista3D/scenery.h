
#pragma once

// CoastVista 3D - scenery.h
// Owner: Ramisa
// Day 3: Trees, Lamp Posts and Roadside Buildings.

#include "scene.h"

// Trees
void drawTree(float x, float z, float scale = 1.0f);
void drawTrees();

// Lamp posts
void drawLampPost(float x, float z);
void drawLampPosts();

// Roadside buildings
void drawHotel(float x, float z);
void drawResort(float x, float z);
void drawRestaurant(float x, float z);

void drawRoadsideBuildings();
