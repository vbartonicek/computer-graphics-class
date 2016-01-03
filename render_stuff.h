// -----------------------------------------------------------
/**
 * \file render_stuff.h
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief Definitions of render functions
*/
// ----------------------------------------------------------

#ifndef __RENDER_STUFF_H
#define __RENDER_STUFF_H
#pragma once
#include "meshGeometry.h"
#include "data.h"
#include "object.h"
#include "animatedTexture.h"
#include "movingTexture.h"


typedef struct FreeCameraObject {
  glm::vec3 position;
  glm::vec3 direction;

} FreeCameraObject;

typedef struct _commonShaderProgram {
	// identifier for the program
	GLuint program;          // = 0;
	// vertex attributes locations
	GLint posLocation;       // = -1;
	GLint colorLocation;     // = -1;
	GLint normalLocation;    // = -1;
	GLint texCoordLocation;  // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;    // = -1;
	GLint VmatrixLocation;      // = -1;  view/camera matrix
	GLint MmatrixLocation;      // = -1;  modeling matrix
	GLint normalMatrixLocation; // = -1;  inverse transposed VMmatrix

	GLint timeLocation;         // = -1; elapsed time in seconds

	// material 
	GLint diffuseLocation;    // = -1;
	GLint ambientLocation;    // = -1;
	GLint specularLocation;   // = -1;
	GLint shininessLocation;  // = -1;
	// texture
	GLint useTextureLocation; // = -1; 
	GLint texSamplerLocation; // = -1;
	// reflector related uniforms
	GLint reflectorPositionLocation;  // = -1; 
	GLint reflectorDirectionLocation; // = -1;
	// fog
	GLint fogOnOffLocation;	
	GLint fogTypeLocation;
	GLint fogTimeLocation;		
	// lights on/off
	GLint sunOnOffLocation;	
	GLint pointOnOffLocation;
	GLint spotOnOffLocation;
	// sun 
	GLint sunAmbientLocation;
	GLint sunDiffuseLocation;
	GLint sunSpecularLocation;
	GLint sunSpeedLocation;

} SCommonShaderProgram;


//Draw models
void drawPub(bartovra::Object* Pub, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawPodloubi(bartovra::Object* Podloubi, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawChapel(bartovra::Object* Chapel, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawFountain(bartovra::Object* Fountain, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawWindmill(bartovra::Object* Windmill, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawFence(bartovra::Object* Fence, int stencislNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawCross(bartovra::Object* Cross, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawBase(bartovra::Object* Base, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawPig(bartovra::Object* Pig, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawPigCurve(bartovra::Object* PigCurve, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawBat(bartovra::Object* Bat, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawTree(bartovra::Object* Tree_leaves, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);

void drawSmoke(bartovra::AniTex* smoke, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawCloud(bartovra::MovTex* Cloud, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);

void PGRdrawSkybox(const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);

void initializeShaderPrograms();
void cleanupShaderPrograms();

void initializeModels();
void cleanupModels();

void setFogUniforms(int fogOnOff,int fogType,bool fogTime);
void setLightUniforms(bool sunOnOff, bool pointOnOff, bool spotOnOff, const glm::vec3 &sunAmbient, const glm::vec3 &sunDiffuse, const glm::vec3 &sunSpecular, float sunSpeed);
#endif // __RENDER_STUFF_H