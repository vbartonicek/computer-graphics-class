// -----------------------------------------------------------
/**
 * \file render_stuff.cpp
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief Render functions
*/
// ----------------------------------------------------------

#include <iostream>
#include "pgr.h"
#include "render_stuff.h"
#include "data.h"
#include "spline.h"


// Mesh geometries
MeshGeometry* PubGeometry = NULL;
MeshGeometry* PodloubiGeometry = NULL;
MeshGeometry* ChapelGeometry = NULL;
MeshGeometry* FountainGeometry = NULL;
MeshGeometry* WindmillGeometry = NULL;
MeshGeometry* FenceGeometry = NULL;
MeshGeometry* CrossGeometry = NULL;
MeshGeometry* BaseGeometry = NULL;
MeshGeometry* PigGeometry = NULL;
MeshGeometry* Pig2Geometry = NULL;
MeshGeometry* BatGeometry = NULL;
MeshGeometry* GroundGeometry = NULL;
MeshGeometry* BushGeometry = NULL;
MeshGeometry* PokusGeometry = NULL;
MeshGeometry* SmokeGeometry = NULL;
MeshGeometry* CompassGeometry = NULL;
MeshGeometry* CloudGeometry = NULL;
MeshGeometry* PGRskyboxGeometry = NULL;

//Objects path
const char* PUB_MODEL_NAME = "data/models/housePub/house_obj.obj";
const char* PODLOUBI_MODEL_NAME = "data/models/houseMedieval/Medieval_House.obj";
const char* CHAPEL_MODEL_NAME = "data/models/houseChapel/chapel_obj.obj";
const char* FOUNTAIN_MODEL_NAME = "data/models/fountain/fountain.obj";
const char* WINDMILL_MODEL_NAME = "data/models/windmill/windmill.obj";
const char* FENCE_MODEL_NAME = "data/models/fence/Old_Fence.obj";
const char* CROSS_MODEL_NAME = "data/models/cross/cross.obj";
const char* BASE_MODEL_NAME = "data/models/ground/ground.obj";
const char* PIG_MODEL_NAME = "data/models/pig/pig.obj";
const char* PIG2_MODEL_NAME = "data/models/pig/pig.obj";
const char* BAT_MODEL_NAME = "data/models/bat/bat.obj";
//const char* BUSH_MODEL_NAME = "data/models/lumberjack/lumberJack.obj";
const char* BUSH_MODEL_NAME = "data/models/bush/SH20_1.obj";

//Textures path
const char* SMOKE_TEXTURE_NAME = "data/textures/smoke/smoke_move_square.png";
const char* CLOUD_TEXTURE_NAME = "data/textures/cloud/cloud2.png";
const char* PGR_SKYBOX_CUBE_TEXTURE_FILE_PREFIX = "data/textures/skybox/skybox";


SCommonShaderProgram shaderProgram;

bool useLighting = false;

//Skybox texture shader program
struct PGRskyboxFarPlaneShaderProgram {
	// identifier for the program
	GLuint program;                 // = 0;
	// vertex attributes locations
	GLint screenCoordLocation;      // = -1;
	// uniforms locations
	GLint inversePVmatrixLocation; // = -1;
	GLint skyboxSamplerLocation;    // = -1;
	GLint timeLocation;          // = -1;
	// fog
	GLint fogOnOffLocation;	
	GLint fogTypeLocation;		
} PGRskyboxFarPlaneShaderProgram;

//Animated texture shader program
struct AniTexShaderProgram {
	// identifier for the program
	GLuint program;              // = 0;
	// vertex attributes locations
	GLint posLocation;           // = -1;
	GLint texCoordLocation;      // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;     // = -1;
	GLint VmatrixLocation;       // = -1;
	GLint timeLocation;          // = -1;
	GLint texSamplerLocation;    // = -1;
	GLint frameDurationLocation; // = -1;
	// fog
	GLint fogOnOffLocation;	
	GLint fogTypeLocation;	
	GLint fogTimeLocation;	

} AniTexShaderProgram;

//Moving texture shader program
struct MovTexShaderProgram {
	// identifier for the program
	GLuint program;           // = 0;
	// vertex attributes locations
	GLint posLocation;        // = -1;
	GLint texCoordLocation;   // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;  // = -1;
	GLint timeLocation;       // = -1;
	GLint texSamplerLocation; // = -1;
	// fog
	GLint fogOnOffLocation;	
	GLint fogTypeLocation;	
	GLint fogTimeLocation;	
} MovTexShaderProgram;

void setTransformUniforms(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
	glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

	glUniformMatrix4fv(shaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
	glUniformMatrix4fv(shaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));  // correct matrix for non-rigid transform

}

void setMaterialUniforms(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess, GLuint texture) {
	glUniform3fv(shaderProgram.diffuseLocation,  1, glm::value_ptr(diffuse));  // 2nd parameter must be 1 - it declares number of vectors in the vector array
	glUniform3fv(shaderProgram.ambientLocation,  1, glm::value_ptr(ambient));
	glUniform3fv(shaderProgram.specularLocation, 1, glm::value_ptr(specular));
	glUniform1f(shaderProgram.shininessLocation,    shininess);

	if(texture != 0) {
		glUniform1i(shaderProgram.useTextureLocation, 1);  // do texture sampling
		glUniform1i(shaderProgram.texSamplerLocation, 0);  // texturing unit 0 -> samplerID   [for the GPU linker]
		glActiveTexture(GL_TEXTURE0 + 0);                  // texturing unit 0 -> to be bound [for OpenGL BindTexture]
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else {
		glUniform1i(shaderProgram.useTextureLocation, 0);  // do not sample the texture
	}


}

void setFogUniforms(int fogOnOff,int fogType,bool fogTime)
{
	glUseProgram(shaderProgram.program);
	glUniform1i(shaderProgram.fogOnOffLocation,fogOnOff);
	glUniform1i(shaderProgram.fogTypeLocation, fogType);
	glUniform1i(shaderProgram.fogTimeLocation, fogTime);

	glUseProgram(PGRskyboxFarPlaneShaderProgram.program);
	glUniform1i(PGRskyboxFarPlaneShaderProgram.fogOnOffLocation,fogOnOff);
	glUniform1i(PGRskyboxFarPlaneShaderProgram.fogTypeLocation, fogType);

	glUseProgram(AniTexShaderProgram.program);
	glUniform1i(AniTexShaderProgram.fogOnOffLocation,fogOnOff);
	glUniform1i(AniTexShaderProgram.fogTypeLocation, fogType);
	glUniform1i(AniTexShaderProgram.fogTimeLocation, fogTime);

	glUseProgram(MovTexShaderProgram.program);
	glUniform1i(MovTexShaderProgram.fogOnOffLocation,fogOnOff);
	glUniform1i(MovTexShaderProgram.fogTypeLocation, fogType);
	glUniform1i(MovTexShaderProgram.fogTimeLocation, fogTime);

	glUseProgram(0);
}

void setLightUniforms(bool sunOnOff, bool pointOnOff)
{
	glUseProgram(shaderProgram.program);
	glUniform1i(shaderProgram.sunOnOffLocation,sunOnOff);
	glUniform1i(shaderProgram.pointOnOffLocation,pointOnOff);
	glUseProgram(0);
}

//functions to draw objects
void drawPub(bartovra::Object *Pub, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Pub->getPosition());
  modelMatrix = glm::rotate(modelMatrix, Pub->getViewAngle(), glm::vec3(0, 1, 0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(Pub->getSize(), Pub->getSize(), Pub->getSize()));

  // setting matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  setMaterialUniforms(
	  PubGeometry->ambient,
	  PubGeometry->diffuse,
	  PubGeometry->specular,
	  PubGeometry->shininess,
	  PubGeometry->texture
	  );

  glBindVertexArray(PubGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, PubGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawPodloubi(bartovra::Object * Podloubi, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Podloubi->getPosition());
  modelMatrix = glm::rotate(modelMatrix, Podloubi->getViewAngle(), glm::vec3(0, 1, 0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(Podloubi->getSize(), Podloubi->getSize(), Podloubi->getSize()));

  // setting matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  setMaterialUniforms(
	  PodloubiGeometry->ambient,
	  PodloubiGeometry->diffuse,
	  PodloubiGeometry->specular,
	  PodloubiGeometry->shininess,
	  PodloubiGeometry->texture
	  );

  glBindVertexArray(PodloubiGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, PodloubiGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawChapel(bartovra::Object *Chapel, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Chapel->getPosition());
  modelMatrix = glm::rotate(modelMatrix, Chapel->getViewAngle(), glm::vec3(0, 1, 0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(Chapel->getSize(), Chapel->getSize(), Chapel->getSize()));

  // setting matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  setMaterialUniforms(
	  ChapelGeometry->ambient,
	  ChapelGeometry->diffuse,
	  ChapelGeometry->specular,
	  ChapelGeometry->shininess,
	  ChapelGeometry->texture
	  );


  glBindVertexArray(ChapelGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, ChapelGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawFountain(bartovra::Object *Fountain, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Fountain->getPosition());
	modelMatrix = glm::rotate(modelMatrix, Fountain->getViewAngle(), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(Fountain->getSize(), Fountain->getSize(), Fountain->getSize()));

	// setting matrices to the vertex & fragment shader
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

	setMaterialUniforms(
		FountainGeometry->ambient,
		FountainGeometry->diffuse,
		FountainGeometry->specular,
		FountainGeometry->shininess,
		FountainGeometry->texture
		);

	glBindVertexArray(FountainGeometry->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, FountainGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

void drawWindmill(bartovra::Object *Windmill, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Windmill->getPosition());
	modelMatrix = glm::rotate(modelMatrix, Windmill->getViewAngle(), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(Windmill->getSize(), Windmill->getSize(), Windmill->getSize()));

	// setting matrices to the vertex & fragment shader
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

	setMaterialUniforms(
		WindmillGeometry->ambient,
		WindmillGeometry->diffuse,
		WindmillGeometry->specular,
		WindmillGeometry->shininess,
		WindmillGeometry->texture
		);

	glBindVertexArray(WindmillGeometry->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, WindmillGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

void drawFence(bartovra::Object *Fence, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Fence->getPosition());
  modelMatrix = glm::rotate(modelMatrix, Fence->getViewAngle(), glm::vec3(0, 1, 0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(Fence->getSize(), Fence->getSize(), Fence->getSize()));

  // setting matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  setMaterialUniforms(
	  FenceGeometry->ambient,
	  FenceGeometry->diffuse,
	  FenceGeometry->specular,
	  FenceGeometry->shininess,
	  FenceGeometry->texture
	  );

  glBindVertexArray(FenceGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, FenceGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

 
  return;
}

void drawCross(bartovra::Object *Cross, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Cross->getPosition());
  modelMatrix = glm::rotate(modelMatrix, Cross->getViewAngle(), glm::vec3(0, 1, 0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(Cross->getSize(), Cross->getSize(), Cross->getSize()));

  // setting matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  setMaterialUniforms(
	  CrossGeometry->ambient,
	  CrossGeometry->diffuse,
	  CrossGeometry->specular,
	  CrossGeometry->shininess,
	  CrossGeometry->texture
	  );

  glBindVertexArray(CrossGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, CrossGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawBase(bartovra::Object *Base, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Base->getPosition());
  modelMatrix = glm::rotate(modelMatrix, Base->getViewAngle(), glm::vec3(0, 1, 0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(Base->getSize(), Base->getSize(), Base->getSize()));

  // setting matrices to the vertex & fragment shader
  setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

  setMaterialUniforms(
	  BaseGeometry->ambient,
	  BaseGeometry->diffuse,
	  BaseGeometry->specular,
	  BaseGeometry->shininess,
	  BaseGeometry->texture
	  );

  glBindVertexArray(BaseGeometry->vertexArrayObject);
  glDrawElements(GL_TRIANGLES, BaseGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  return;
}

void drawPig(bartovra::Object *Pig, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Pig->getPosition());
	modelMatrix = glm::rotate(modelMatrix, Pig->getViewAngle(), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(Pig->getSize(), Pig->getSize(), Pig->getSize()));

	// setting matrices to the vertex & fragment shader
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

	setMaterialUniforms(
		PigGeometry->ambient,
		PigGeometry->diffuse,
		PigGeometry->specular,
		PigGeometry->shininess,
		PigGeometry->texture
		);

	glBindVertexArray(PigGeometry->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, PigGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

void drawPigCurve(bartovra::Object *PigCurve, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

	// align ufo coordinate system to match its position and direction - see alignObject() function
	glm::mat4 modelMatrix = alignObject(PigCurve->getPosition(), PigCurve->getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PigCurve->getSize()));

	//glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), PigCurve->getPosition());
	//modelMatrix = glm::rotate(modelMatrix, PigCurve->getViewAngle(), glm::vec3(0, 1, 0));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(Pig->getSize(), PigCurve->getSize(), PigCurve->getSize()));

	// setting matrices to the vertex & fragment shader
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

	setMaterialUniforms(
		Pig2Geometry->ambient,
		Pig2Geometry->diffuse,
		Pig2Geometry->specular,
		Pig2Geometry->shininess,
		Pig2Geometry->texture
		);

	glBindVertexArray(Pig2Geometry->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, Pig2Geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

void drawBat(bartovra::Object *Bat, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);


	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Bat->getPosition());
	modelMatrix = glm::rotate(modelMatrix, Bat->getViewAngle(), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(Bat->getSize(), Bat->getSize(), Bat->getSize()));

	// align bat coordinate system to match its position and direction - see alignObject() function
	//glm::mat4 modelMatrix = alignObject(Bat->getPosition(), glm::vec3(0.0f,Bat->getViewAngle(),0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(Bat->getSize(), Bat->getSize(), Bat->getSize()));

	// setting matrices to the vertex & fragment shader
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

	// angular speed = 2*pi*frequency => path = angular speed * time
	//const float frequency = 0.33f; // per second
	//float angle = 6.28f * frequency * (Bat->getCurrentTime()-Bat->getStartTime()); // angle in radians
	//float scaleFactor = 0.5f*(cos(angle) + 1.0f);

	setMaterialUniforms(
		BatGeometry->ambient,
		BatGeometry->diffuse,
		BatGeometry->specular,
		BatGeometry->shininess,
		BatGeometry->texture
		);

	glBindVertexArray(BatGeometry->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, BatGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}


void drawSmoke(bartovra::AniTex* Smoke, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram(AniTexShaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

	// just take rotation part of the view transform
	glm::mat4 billboardRotationMatrix = glm::mat4(
		viewMatrix[0],
		viewMatrix[1],
		viewMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
	// inverse view rotation
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), Smoke->getPosition());
	matrix = glm::scale(matrix, glm::vec3(Smoke->getSize()));
	matrix = matrix*billboardRotationMatrix; // make billboard to face the camera

	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;
	glUniformMatrix4fv(AniTexShaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
	glUniformMatrix4fv(AniTexShaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));   // view
	glUniform1f(AniTexShaderProgram.timeLocation, Smoke->getCurrentTime() - Smoke->getStartTime());
	glUniform1i(AniTexShaderProgram.texSamplerLocation, 0);
	glUniform1f(AniTexShaderProgram.frameDurationLocation, Smoke->getFrameDuration());

	glBindVertexArray(SmokeGeometry->vertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, SmokeGeometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, SmokeGeometry->numTriangles);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);

	return;
}

void drawCloud(bartovra::MovTex* Cloud, int stencilNum, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(MovTexShaderProgram.program);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), Cloud->getPosition());
	matrix = glm::scale(matrix, glm::vec3(Cloud->getSize()));

	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;
	glUniformMatrix4fv(MovTexShaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));        // model-view-projection
	glUniform1f(MovTexShaderProgram.timeLocation, Cloud->getCurrentTime() - Cloud->getStartTime());
	glUniform1i(MovTexShaderProgram.texSamplerLocation, 0);

	glBindTexture(GL_TEXTURE_2D, CloudGeometry->texture);
	glBindVertexArray(CloudGeometry->vertexArrayObject);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, CloudGeometry->numTriangles);

	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);

	return;
}

void PGRdrawSkybox(const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(PGRskyboxFarPlaneShaderProgram.program);

	// compose transformations
	glm::mat4 matrix = projectionMatrix * viewMatrix;

	// crate view rotation matrix by using view matrix with cleared translation
	glm::mat4 viewRotation = -viewMatrix;
	viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// vertex shader will translate screen space coordinates (NDC) using inverse PV matrix
	glm::mat4 inversePVmatrix = glm::inverse(projectionMatrix * viewRotation);

	glUniformMatrix4fv(PGRskyboxFarPlaneShaderProgram.inversePVmatrixLocation, 1, GL_FALSE, glm::value_ptr(inversePVmatrix));
	glUniform1i(PGRskyboxFarPlaneShaderProgram.skyboxSamplerLocation, 0);

	// draw "PGRskybox" rendering 2 triangles covering the far plane
	glBindVertexArray(PGRskyboxGeometry->vertexArrayObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PGRskyboxGeometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, PGRskyboxGeometry->numTriangles+2);

	glBindVertexArray(0);
	glUseProgram(0);
}

void drawBush(bartovra::Object *Bush, int stencilNum,const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {

	glUseProgram(shaderProgram.program);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, stencilNum, -1);

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Bush->getPosition());
	modelMatrix = glm::rotate(modelMatrix, Bush->getViewAngle(), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(Bush->getSize(), Bush->getSize(), Bush->getSize()));

	// setting matrices to the vertex & fragment shader
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);

	setMaterialUniforms(
		BushGeometry->ambient,
		BushGeometry->diffuse,
		BushGeometry->specular,
		BushGeometry->shininess,
		BushGeometry->texture
		);

	glBindVertexArray(BushGeometry->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, BushGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}



void cleanupShaderPrograms(void) {
	pgr::deleteProgramAndShaders(shaderProgram.program);
	pgr::deleteProgramAndShaders(PGRskyboxFarPlaneShaderProgram.program);
	pgr::deleteProgramAndShaders(AniTexShaderProgram.program);
	pgr::deleteProgramAndShaders(MovTexShaderProgram.program);
}

//Shaders initialization
void initializeShaderPrograms(void) {

	std::vector<GLuint> shaderList;

	if(useLighting == true) {
		// load and compile shader for lighting (lights & materials)

		// push vertex shader and fragment shader

		shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/textureLightShader.vert"));
		shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/textureLightShader.frag"));
		//shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/lightShader.vert"));
		//shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/lightShader.frag"));

		// create the program with two shaders
		shaderProgram.program = pgr::createProgram(shaderList);

		// get vertex attributes locations, if the shader does not have this uniform -> return -1
		shaderProgram.posLocation      = glGetAttribLocation(shaderProgram.program, "position");
		shaderProgram.normalLocation   = glGetAttribLocation(shaderProgram.program, "normal");
		shaderProgram.texCoordLocation = glGetAttribLocation(shaderProgram.program, "texCoord");
		// get uniforms locations
		shaderProgram.PVMmatrixLocation    = glGetUniformLocation(shaderProgram.program, "PVMmatrix");
		shaderProgram.VmatrixLocation      = glGetUniformLocation(shaderProgram.program, "Vmatrix");
		shaderProgram.MmatrixLocation      = glGetUniformLocation(shaderProgram.program, "Mmatrix");
		shaderProgram.normalMatrixLocation = glGetUniformLocation(shaderProgram.program, "normalMatrix");
		shaderProgram.timeLocation         = glGetUniformLocation(shaderProgram.program, "time");
		// material
		shaderProgram.ambientLocation      = glGetUniformLocation(shaderProgram.program, "material.ambient");
		shaderProgram.diffuseLocation      = glGetUniformLocation(shaderProgram.program, "material.diffuse");
		shaderProgram.specularLocation     = glGetUniformLocation(shaderProgram.program, "material.specular");
		shaderProgram.shininessLocation    = glGetUniformLocation(shaderProgram.program, "material.shininess");
		// texture
		shaderProgram.texSamplerLocation   = glGetUniformLocation(shaderProgram.program, "texSampler");
		shaderProgram.useTextureLocation   = glGetUniformLocation(shaderProgram.program, "material.useTexture");
		// reflector
		shaderProgram.reflectorPositionLocation  = glGetUniformLocation(shaderProgram.program, "reflectorPosition");
		shaderProgram.reflectorDirectionLocation = glGetUniformLocation(shaderProgram.program, "reflectorDirection");
		// fog
		shaderProgram.fogOnOffLocation			= glGetUniformLocation(shaderProgram.program, "fog.fogOnOff");
		shaderProgram.fogTypeLocation			= glGetUniformLocation(shaderProgram.program, "fog.fogType");
		shaderProgram.fogTimeLocation			= glGetUniformLocation(shaderProgram.program, "fog.fogTime");
		//lights
		shaderProgram.sunOnOffLocation			= glGetUniformLocation(shaderProgram.program, "sunOnOff");
		shaderProgram.pointOnOffLocation			= glGetUniformLocation(shaderProgram.program, "pointOnOff");

	}
	else {
		// load and compile simple shader (colors only, no lights at all)

		// push vertex shader and fragment shader
		shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/colorShader.vert"));
		shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/colorShader.frag"));

		// create the program with two shaders (fragment and vertex)
		shaderProgram.program = pgr::createProgram(shaderList);
		// get position and color attributes locations
		shaderProgram.posLocation = glGetAttribLocation(shaderProgram.program, "position");
		shaderProgram.colorLocation = glGetAttribLocation(shaderProgram.program, "color");
		// get uniforms locations
		shaderProgram.PVMmatrixLocation    = glGetUniformLocation(shaderProgram.program, "PVMmatrix");

	}

	// load and compile shader for smoke (dynamic texture)

	shaderList.clear();

	// push vertex shader and fragment shader
	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/aniTexShader.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/aniTexShader.frag"));

	// create the program with two shaders
	AniTexShaderProgram.program = pgr::createProgram(shaderList);

	// get position and texture coordinates attributes locations
	AniTexShaderProgram.posLocation      = glGetAttribLocation(AniTexShaderProgram.program, "position");
	AniTexShaderProgram.texCoordLocation = glGetAttribLocation(AniTexShaderProgram.program, "texCoord");
	// get uniforms locations
	AniTexShaderProgram.PVMmatrixLocation     = glGetUniformLocation(AniTexShaderProgram.program, "PVMmatrix");
	AniTexShaderProgram.VmatrixLocation       = glGetUniformLocation(AniTexShaderProgram.program, "Vmatrix");
	AniTexShaderProgram.timeLocation          = glGetUniformLocation(AniTexShaderProgram.program, "time");
	AniTexShaderProgram.texSamplerLocation    = glGetUniformLocation(AniTexShaderProgram.program, "texSampler");
	AniTexShaderProgram.frameDurationLocation = glGetUniformLocation(AniTexShaderProgram.program, "frameDuration");
	// fog
	AniTexShaderProgram.fogOnOffLocation			= glGetUniformLocation(AniTexShaderProgram.program, "fog.fogOnOff");
	AniTexShaderProgram.fogTypeLocation			= glGetUniformLocation(AniTexShaderProgram.program, "fog.fogType");
	AniTexShaderProgram.fogTimeLocation			= glGetUniformLocation(AniTexShaderProgram.program, "fog.fogTime");


	// load and compile shader for CLOUD

	shaderList.clear();

	// push vertex shader and fragment shader
	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/movTexShader.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/movTexShader.frag"));

	// Create the program with two shaders
	MovTexShaderProgram.program = pgr::createProgram(shaderList);

	// get position and color attributes locations
	MovTexShaderProgram.posLocation      = glGetAttribLocation(MovTexShaderProgram.program, "position");
	MovTexShaderProgram.texCoordLocation = glGetAttribLocation(MovTexShaderProgram.program, "texCoord");
	// get uniforms locations
	MovTexShaderProgram.PVMmatrixLocation  = glGetUniformLocation(MovTexShaderProgram.program, "PVMmatrix");
	MovTexShaderProgram.timeLocation       = glGetUniformLocation(MovTexShaderProgram.program, "time");
	MovTexShaderProgram.texSamplerLocation = glGetUniformLocation(MovTexShaderProgram.program, "texSampler");
	// fog
	MovTexShaderProgram.fogOnOffLocation			= glGetUniformLocation(MovTexShaderProgram.program, "fog.fogOnOff");
	MovTexShaderProgram.fogTypeLocation			= glGetUniformLocation(MovTexShaderProgram.program, "fog.fogType");
	MovTexShaderProgram.fogTimeLocation			= glGetUniformLocation(MovTexShaderProgram.program, "fog.fogTime");




	// load and compile shader for PGRskybox (cube map)

	shaderList.clear();

	// push vertex shader and fragment shader
	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skyboxShader.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skyboxShader.frag"));

	// create the program with two shaders
	PGRskyboxFarPlaneShaderProgram.program = pgr::createProgram(shaderList);

	// handles to vertex attributes locations
	PGRskyboxFarPlaneShaderProgram.screenCoordLocation = glGetAttribLocation(PGRskyboxFarPlaneShaderProgram.program, "screenCoord");
	// get uniforms locations
	PGRskyboxFarPlaneShaderProgram.skyboxSamplerLocation   = glGetUniformLocation(PGRskyboxFarPlaneShaderProgram.program, "skyboxSampler");
	PGRskyboxFarPlaneShaderProgram.inversePVmatrixLocation = glGetUniformLocation(PGRskyboxFarPlaneShaderProgram.program, "inversePVmatrix");
	// fog
	PGRskyboxFarPlaneShaderProgram.fogOnOffLocation			= glGetUniformLocation(PGRskyboxFarPlaneShaderProgram.program, "fog.fogOnOff");
	PGRskyboxFarPlaneShaderProgram.fogTypeLocation			= glGetUniformLocation(PGRskyboxFarPlaneShaderProgram.program, "fog.fogType");
}

bool loadSingleMesh(const std::string &fileName, SCommonShaderProgram& shader, MeshGeometry** geometry) {
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1); // Unitize object in size (scale the model to fit into (-1..1)^3)
	// Load asset from the file - you can play with various processing steps
	const aiScene * scn = importer.ReadFile(fileName.c_str(), 0
		| aiProcess_Triangulate             // Triangulate polygons (if any).
		| aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
		| aiProcess_GenSmoothNormals        // Calculate normals per vertex.
		| aiProcess_JoinIdenticalVertices);
	// abort if the loader fails
	if(scn == NULL) {
		std::cerr << "assimp error: " << importer.GetErrorString() << std::endl;
		*geometry = NULL;
		return false;
	}
	// some formats store whole scene (multiple meshes and materials, lights, cameras, ...) in one file, we cannot handle that in our simplified example
	if(scn->mNumMeshes != 1) {
		std::cerr << "this simplified loader can only process files with only one mesh" << std::endl;
		*geometry = NULL;
		return false;
	}
	// in this phase we know we have one mesh in our loaded scene, we can directly copy its data to opengl ...
	const aiMesh * mesh = scn->mMeshes[0];

	*geometry = new MeshGeometry;

	// vertex buffer object, store all vertex positions and normals
	glGenBuffers(1, &((*geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float)*mesh->mNumVertices, 0, GL_STATIC_DRAW); // allocate memory for vertices, normals, and texture coordinates
	// first store all vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3*sizeof(float)*mesh->mNumVertices, mesh->mVertices);
	// then store all normals
	glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*mesh->mNumVertices, 3*sizeof(float)*mesh->mNumVertices, mesh->mNormals);

	// just texture 0 for now
	float *textureCoords = new float[2 * mesh->mNumVertices];  // 2 floats per vertex
	float *currentTextureCoord = textureCoords;

	// copy texture coordinates
	aiVector3D vect;

	if(mesh->HasTextureCoords(0) ) {
		// we use 2D textures with 2 coordinates and ignore the third coordinate
		for(unsigned int idx=0; idx<mesh->mNumVertices; idx++) {
			vect = (mesh->mTextureCoords[0])[idx];
			*currentTextureCoord++ = vect.x;
			*currentTextureCoord++ = vect.y;
		}
	}

	// finally store all texture coordinates
	glBufferSubData(GL_ARRAY_BUFFER, 6*sizeof(float)*mesh->mNumVertices, 2*sizeof(float)*mesh->mNumVertices, textureCoords);

	// copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
	unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
	for(unsigned int f = 0; f < mesh->mNumFaces; ++f) {
		indices[f*3 + 0] = mesh->mFaces[f].mIndices[0];
		indices[f*3 + 1] = mesh->mFaces[f].mIndices[1];
		indices[f*3 + 2] = mesh->mFaces[f].mIndices[2];
	}
	// copy our temporary index array to OpenGL and free the array
	glGenBuffers(1, &((*geometry)->elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

	delete [] indices;

	// copy the material info to MeshGeometry structure
	const aiMaterial *mat  = scn->mMaterials[mesh->mMaterialIndex];
	aiColor3D color;
	aiString name;

	// Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
	mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing. Must be aiString type!

	mat->Get<aiColor3D>(AI_MATKEY_COLOR_DIFFUSE, color);
	(*geometry)->diffuse = glm::vec3(color.r, color.g, color.b);

	// mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mat->Get<aiColor3D>(AI_MATKEY_COLOR_AMBIENT, color);
	(*geometry)->ambient = glm::vec3(color.r, color.g, color.b);

	mat->Get<aiColor3D>(AI_MATKEY_COLOR_SPECULAR, color);
	(*geometry)->specular = glm::vec3(color.r, color.g, color.b);

	float shininess;

	mat->Get<float>(AI_MATKEY_SHININESS, shininess);
	(*geometry)->shininess = shininess / 4.0f;  // shininess divisor-not descibed anywhere

	(*geometry)->texture = 0;

	// load texture image
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		// get texture name 
		mat->Get<aiString>(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0) , name);
		std::string textureName = name.data;

		size_t found = fileName.find_last_of("/\\");
		// insert correct texture file path 
		if(found != std::string::npos) { // not found
			//subMesh_p->textureName.insert(0, "/");
			textureName.insert(0, fileName.substr(0, found+1));
		}

		std::cout << "Loading texture file: " << textureName << std::endl;
		(*geometry)->texture = pgr::createTexture(textureName);
	}
	CHECK_GL_ERROR();

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
	glBindVertexArray((*geometry)->vertexArrayObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject); // bind our element array buffer (indices) to vao
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);

	glEnableVertexAttribArray(shader.posLocation);
	glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if(useLighting == true) {
		glEnableVertexAttribArray(shader.normalLocation);
		glVertexAttribPointer(shader.normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));
	}
	else {
		glVertexAttrib3f(shader.colorLocation, color.r, color.g, color.b);
	}

	glEnableVertexAttribArray(shader.texCoordLocation);
	glVertexAttribPointer(shader.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));
	CHECK_GL_ERROR();

	glBindVertexArray(0);

	(*geometry)->numTriangles = mesh->mNumFaces;

	return true;
}

void initSmokeGeometry(GLuint shader, MeshGeometry **geometry) {

	*geometry = new MeshGeometry;

	(*geometry)->texture = pgr::createTexture(SMOKE_TEXTURE_NAME);

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
	glBindVertexArray((*geometry)->vertexArrayObject);

	glGenBuffers(1, &((*geometry)->vertexBufferObject));\
		glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smokeVertexData), smokeVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(AniTexShaderProgram.posLocation);
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(AniTexShaderProgram.posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(AniTexShaderProgram.texCoordLocation);
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	glVertexAttribPointer(AniTexShaderProgram.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	(*geometry)->numTriangles = smokeNumQuadVertices;
}

void initCloudGeometry(GLuint shader, MeshGeometry **geometry) {


	*geometry = new MeshGeometry;

	(*geometry)->texture = pgr::createTexture(CLOUD_TEXTURE_NAME);
	glBindTexture(GL_TEXTURE_2D, (*geometry)->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
	glBindVertexArray((*geometry)->vertexArrayObject);

	glGenBuffers(1, &((*geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cloudVertexData), cloudVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(MovTexShaderProgram.posLocation);
	glEnableVertexAttribArray(MovTexShaderProgram.texCoordLocation);
	// vertices of triangles - start at the beginning of the interlaced array
	glVertexAttribPointer(MovTexShaderProgram.posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	// texture coordinates of each vertices are stored just after its position
	glVertexAttribPointer(MovTexShaderProgram.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	(*geometry)->numTriangles = cloudNumQuadVertices;
}

void PGRinitSkyboxGeometry(GLuint shader, MeshGeometry **geometry) {

	*geometry = new MeshGeometry;

	// 2D coordinates of 2 triangles covering the whole screen (NDC), draw using triangle strip
	static const float screenCoords[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f
	};

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
	glBindVertexArray((*geometry)->vertexArrayObject);

	// buffer for far plane rendering
	glGenBuffers(1, &((*geometry)->vertexBufferObject));\
		glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_DYNAMIC_DRAW);

	//glUseProgram(farplaneShaderProgram);

	glEnableVertexAttribArray(PGRskyboxFarPlaneShaderProgram.screenCoordLocation);
	glVertexAttribPointer(PGRskyboxFarPlaneShaderProgram.screenCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();

	(*geometry)->numTriangles = 2;

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &((*geometry)->texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, (*geometry)->texture);

	const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for( int i = 0; i < 6; i++ ) {
		std::string texName = std::string(PGR_SKYBOX_CUBE_TEXTURE_FILE_PREFIX) + "_" + suffixes[i] + ".jpg";
		std::cout << "Loading cube map texture: " << texName << std::endl;
		if(!pgr::loadTexImage2D(texName, targets[i])) {
			pgr::dieWithError("Skybox cube map loading failed!");
		}
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// unbind the texture (just in case someone will mess up with texture calls later)
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	CHECK_GL_ERROR();
}

//Models initialization
void initializeModels() {

  // load pub model from external file
  if(loadSingleMesh(PUB_MODEL_NAME, shaderProgram, &PubGeometry) != true) {
    std::cerr << "initializeModels(): Pub model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load podloubi model from external file
  if(loadSingleMesh(PODLOUBI_MODEL_NAME, shaderProgram, &PodloubiGeometry) != true) {
    std::cerr << "initializeModels(): Podloubi model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load chapel model from external file
  if(loadSingleMesh(CHAPEL_MODEL_NAME, shaderProgram, &ChapelGeometry) != true) {
    std::cerr << "initializeModels(): Chapel model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load fountain model from external file
  if(loadSingleMesh(FOUNTAIN_MODEL_NAME, shaderProgram, &FountainGeometry) != true) {
    std::cerr << "initializeModels(): Fountain model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load windmill model from external file
  if(loadSingleMesh(WINDMILL_MODEL_NAME, shaderProgram, &WindmillGeometry) != true) {
	  std::cerr << "initializeModels(): Windmill model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load fence model from external file
  if(loadSingleMesh(FENCE_MODEL_NAME, shaderProgram, &FenceGeometry) != true) {
    std::cerr << "initializeModels(): Fence model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load cross model from external file
  if(loadSingleMesh(CROSS_MODEL_NAME, shaderProgram, &CrossGeometry) != true) {
    std::cerr << "initializeModels(): Cross model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load Base model from external file
  if(loadSingleMesh(BASE_MODEL_NAME, shaderProgram, &BaseGeometry) != true) {
    std::cerr << "initializeModels(): Base model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load Pig model from external file
  if(loadSingleMesh(PIG_MODEL_NAME, shaderProgram, &PigGeometry) != true) {
	  std::cerr << "initializeModels(): Pig model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load Pig2 model from external file
  if(loadSingleMesh(PIG2_MODEL_NAME, shaderProgram, &Pig2Geometry) != true) {
	  std::cerr << "initializeModels(): Pig2 model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load Bat model from external file
  if(loadSingleMesh(BAT_MODEL_NAME, shaderProgram, &BatGeometry) != true) {
	  std::cerr << "initializeModels(): Bat model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // load Bush model from external file
  if(loadSingleMesh(BUSH_MODEL_NAME, shaderProgram, &BushGeometry) != true) {
	  std::cerr << "initializeModels(): Bush model loading failed." << std::endl;
  }
  CHECK_GL_ERROR();

  // fill MeshGeometry structure for smoke object
  initSmokeGeometry(AniTexShaderProgram.program, &SmokeGeometry);

  // fill MeshGeometry structure for cloud object
  initCloudGeometry(MovTexShaderProgram.program, &CloudGeometry);

  // fill MeshGeometry structure for PGRskybox object
  PGRinitSkyboxGeometry(PGRskyboxFarPlaneShaderProgram.program, &PGRskyboxGeometry);

}

void cleanupGeometry(MeshGeometry *geometry) {
  glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
  glDeleteBuffers(1, &(geometry->elementBufferObject));
  glDeleteBuffers(1, &(geometry->vertexBufferObject));
}

void cleanupModels() {

  cleanupGeometry(PubGeometry);
  cleanupGeometry(PodloubiGeometry);
  cleanupGeometry(ChapelGeometry);
  cleanupGeometry(FountainGeometry);
  cleanupGeometry(WindmillGeometry);
  cleanupGeometry(FenceGeometry);
  cleanupGeometry(CrossGeometry);
  cleanupGeometry(BaseGeometry);
  cleanupGeometry(PigGeometry);
  cleanupGeometry(BatGeometry);
  cleanupGeometry(BushGeometry);

  cleanupGeometry(SmokeGeometry);
  cleanupGeometry(CloudGeometry);

  cleanupGeometry(PGRskyboxGeometry);
}