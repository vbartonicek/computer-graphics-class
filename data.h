// -----------------------------------------------------------
/**
 * \file data.h
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief Definitions of used constants
*/
// ----------------------------------------------------------

#pragma once

#ifndef __DATA_H
#define __DATA_H

#define WINDOW_WIDTH   1024//800
#define WINDOW_HEIGHT  768//600
#define WINDOW_TITLE   "Semestralka"

#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f
#define SCENE_DEPTH  1.0f

#define STEP 0.025f

#define CAMERA_CURVE_SPEED 0.10f

#define FIRST_PERSON_Y 0.125f

// keys used in the key map
enum {
	KEY_LEFT_ARROW, KEY_RIGHT_ARROW,
	KEY_UP_ARROW, KEY_DOWN_ARROW, 
	//KEY_SPACE, 
	KEY_F1,KEY_F2,KEY_F3,KEY_F4,
	KEYS_COUNT };

//menu items
enum {
	WINDMILL_VIEW, GRAVEYARD_VIEW,
	FREE_CAMERA, FREE_FIRST_PERSON_CAMERA,
	CURVE_VIEW,
	SMOKE_ON_OFF,
	FOG_ON_OFF,FOG_LINEAR, FOG_EXPONENTIAL,FOG_TIME,
	SUN_ON_OFF,POINT_ON_OFF,CLOUD_ON_OFF,
    MENU_COUNT
};

//picking items
enum {
	PICKING_WORLD,
	PICKING_COMPASS,

	PICKING_FENCE_1,
	PICKING_FENCE_2,
	PICKING_FENCE_3,
	PICKING_FENCE_4,
	PICKING_FENCE_5,
	PICKING_FENCE_6,
	PICKING_FENCE_7,
	PICKING_FENCE_8,
	PICKING_FENCE_9,

	PICKING_PIG_CURVE,

	PICKING_SMOKE_1,
	PICKING_SMOKE_2,
	PICKING_SMOKE_3,

	PICKING_CLOUD_1,

	PICKING_PIG,
	PICKING_PUB_1,
	PICKING_PUB2,
	PICKING_PODLOUBI, 
	PICKING_CHAPEL,
	PICKING_FOUNTAIN,
	PICKING_WINDMILL,
	PICKING_CROSS,
	PICKING_BASE,
	PICKING_BAT,

	PICKING_TREE_1,
	PICKING_TREE_2,
	PICKING_TREE_3,
	PICKING_TREE_4,
	PICKING_TREE_5,
	PICKING_TREE_6,
	PICKING_TREE_7,
};

//Animated texture of smoke
const int smokeNumQuadVertices = 4;
const float smokeVertexData[smokeNumQuadVertices * 5] = {


	1.0f,  -1.0f, 0.0f, 0.0f, 0.0f,//vlevo dole
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f,//vpravo dole
	-1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,//vlevo nahore
	-1.0f,  1.0f, 0.0f, 1.0f, 1.0f,//vpravo nahore
};

//Compass texture
const int compassNumQuadVertices = 4;
const float compassVertexData[compassNumQuadVertices * 5] = {

	-1.0f,-1.0f, 0.0f, 0.0f, 0.0f,//vlevo dole
	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,//vpravo dole
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,//vlevo nahore
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,//vpravo nahore
};

//Compass texture
const int cloudNumQuadVertices = 4;
const float cloudVertexData[cloudNumQuadVertices * 5] = {
	/*
	-1.0f,-1.0f, 0.0f, 0.0f, 0.0f,//vlevo dole
	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,//vpravo dole
	-1.0f, 1.0f, 0.0f, 0.0f, 0.5f,//vlevo nahore
	1.0f, 1.0f, 0.0f, 1.0f, 0.5f,//vpravo nahore
	*/

	// x      y      z     u     v
	-1.0f,  0.15f, 0.0f, 0.0f, 1.0f,
	-1.0f, -0.15f, 0.0f, 0.0f, 0.0f,
	1.0f,  0.15f, 0.0f, 3.0f, 1.0f,
	1.0f, -0.15f, 0.0f, 3.0f, 0.0f
};




#endif // __DATA_H