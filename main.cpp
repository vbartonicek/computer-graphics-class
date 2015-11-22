// -----------------------------------------------------------
/**
 * \file main.cpp
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief main function + common functions
*/
// ----------------------------------------------------------

#include <time.h>
#include <list>
#include "pgr.h"
#include "render_stuff.h"
#include "spline.h"

extern SCommonShaderProgram shaderProgram;
extern bool useLighting;

struct SceneState {

  float elapsedTime;
  int windowWidth;    // set by reshape
  int windowHeight;   // set by reshape

  int cameraType;
  float CameraCurveStartTime;

  int fogOnOff;
  int fogType;
  bool fogTime;

  bool sunOnOff;
  bool pointOnOff;

  bool smoke_on_off;
  bool cloud_on_off;

  bool keyMap[KEYS_COUNT];    // false
  bool menuMap[MENU_COUNT];    // false


  bool fence4Draw;
  bool fence5Draw;
  bool fence6Draw;
  bool fence7Draw;
  bool fence8Draw;
  bool fence9Draw;
  bool pigCurveDraw;


} SceneState;

struct SceneObjects {
  bartovra::Object *Pub;
  bartovra::Object *Pub2;
  bartovra::Object *Podloubi;
  bartovra::Object *Chapel;
  bartovra::Object *Fountain;
  bartovra::Object *Windmill;
  bartovra::Object *Pig;
  bartovra::Object *Pig2;
  bartovra::Object *Bat;

  bartovra::Object *Fence1;
  bartovra::Object *Fence2;
  bartovra::Object *Fence3;
  bartovra::Object *Fence4;
  bartovra::Object *Fence5;
  bartovra::Object *Fence6;
  bartovra::Object *Fence7;
  bartovra::Object *Fence8;
  bartovra::Object *Fence9;;

  bartovra::Object *Cross;
  bartovra::Object *Base;

  bartovra::AniTex *Smoke1;
  bartovra::AniTex *Smoke2;
  bartovra::AniTex *Smoke3;

  bartovra::MovTex *Cloud;

} SceneObjects;

struct camera {
	glm::mat4 orthoProjectionMatrix;
	glm::mat4 orthoViewMatrix;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 upVector;

	glm::mat4 angleTransform;//mouse

	bool firstPersonMode;        // false;
	bool move;        // false;
	float angleX;
	float angleY;
	bool up;
	bool down;

	bool firstPersonInit;
	bool freeInit;
} camera;


void cleanUpObjects(void) {
}



void reDefineCamera(){
	switch(SceneState.cameraType)
	{
	case WINDMILL_VIEW:
		// static view from windmill
		camera.position = glm::vec3(-1.0f,0.24f,3.48f);
		camera.direction= glm::vec3(0.0f,0.12f,-1.0f);
		camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.angleX=25.0f;
		camera.angleY=0.0f;
		break;
	case GRAVEYARD_VIEW:
		//static view from graveyard
		camera.position = glm::vec3(1.42f,0.72f,-2.4f);
		camera.direction= glm::vec3(0.0f,-0.08f,-1.0f);
		camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.angleX=-150.0f;
		camera.angleY=0.0f;
		break;

	case FREE_FIRST_PERSON_CAMERA:
		//dynamic first person camera
		camera.position = glm::vec3(0.0f,0.2f,2.0f);
		camera.direction= glm::vec3(0.0f,0.2f,-1.0f);
		camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		camera.angleX=0.0f;
		camera.angleY=0.0f;
		//setCompassPosition();
		break;
	}

}

void restartScene(void) {

  cleanUpObjects();

  SceneState.smoke_on_off = true;//true=on
  SceneState.cloud_on_off = false;//true=on
  SceneState.fogOnOff = 0; //0=off, 1=on
  SceneState.fogType = 1;//1=linear, 2=exponential
  SceneState.fogTime=false;

  SceneState.sunOnOff=true;
  SceneState.sunOnOff=false;

  SceneState.fence4Draw = true;
  SceneState.fence5Draw = true;
  SceneState.fence6Draw = true;
  SceneState.fence7Draw = true;
  SceneState.fence8Draw = true;
  SceneState.fence9Draw = true;
  SceneState.pigCurveDraw = true;

  SceneState.cameraType=WINDMILL_VIEW;

  SceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds

  camera.move=false;
  camera.angleX=0;
  camera.angleY=0;
  camera.firstPersonInit=false;
  camera.freeInit=false;
  camera.up=false;
  camera.down=false;
  reDefineCamera();
  
  //initialise objects
  if(SceneObjects.Pub == NULL) SceneObjects.Pub = new bartovra::Object(glm::vec3(-1.1f, 0.36f, 0.7f),90.0f,0.6f,SceneState.elapsedTime);
  if(SceneObjects.Pub2 == NULL) SceneObjects.Pub2 = new bartovra::Object(glm::vec3(1.1f, 0.36f, 0.33f),-63.0f,0.6f,SceneState.elapsedTime);
  if(SceneObjects.Podloubi == NULL) SceneObjects.Podloubi = new bartovra::Object(glm::vec3(1.35f, 0.5f, 1.40f),-100.0f,0.55f,SceneState.elapsedTime);
  if(SceneObjects.Chapel == NULL) SceneObjects.Chapel = new bartovra::Object(glm::vec3(0.15f, 0.85f, -1.0f),90.0f,0.8f,SceneState.elapsedTime);
  if(SceneObjects.Fountain == NULL) SceneObjects.Fountain = new bartovra::Object(glm::vec3(-0.05f, 0.14f, 0.9f),0.0f,0.15f,SceneState.elapsedTime);
  if(SceneObjects.Windmill == NULL) SceneObjects.Windmill = new bartovra::Object(glm::vec3(-1.1f, 0.85f, 1.7f),0.0f,0.8f,SceneState.elapsedTime);
  if(SceneObjects.Fence1 == NULL) SceneObjects.Fence1 = new bartovra::Object(glm::vec3(0.55f, 0.13f, -1.65f),0.0f,0.12f,SceneState.elapsedTime);//at chapel
  if(SceneObjects.Fence2 == NULL) SceneObjects.Fence2 = new bartovra::Object(glm::vec3(0.78f, 0.13f, -1.65f),0.0f,0.12f,SceneState.elapsedTime);
  if(SceneObjects.Fence3 == NULL) SceneObjects.Fence3 = new bartovra::Object(glm::vec3(1.01f, 0.13f, -1.65f),0.0f,0.12f,SceneState.elapsedTime);
  if(SceneObjects.Fence4 == NULL) SceneObjects.Fence4 = new bartovra::Object(glm::vec3(1.12f, 0.13f, -1.55f),90.0f,0.12f,SceneState.elapsedTime);
  if(SceneObjects.Fence5 == NULL) SceneObjects.Fence5 = new bartovra::Object(glm::vec3(1.12f, 0.13f, -1.32f),90.0f,0.12f,SceneState.elapsedTime);
  if(SceneObjects.Fence6 == NULL) SceneObjects.Fence6 = new bartovra::Object(glm::vec3(1.12f, 0.13f, -1.09f),90.0f,0.12f,SceneState.elapsedTime);
  if(SceneObjects.Fence7 == NULL) SceneObjects.Fence7 = new bartovra::Object(glm::vec3(1.12f, 0.13f, -0.86f),90.0f,0.12f,SceneState.elapsedTime);
  if(SceneObjects.Fence8 == NULL) SceneObjects.Fence8 = new bartovra::Object(glm::vec3(1.12f, 0.13f, -0.63f),90.0f,0.12f,SceneState.elapsedTime);
  if(SceneObjects.Fence9 == NULL) SceneObjects.Fence9 = new bartovra::Object(glm::vec3(1.12f, 0.13f, -0.40f),90.0f,0.12f,SceneState.elapsedTime);//at pub
  if(SceneObjects.Cross == NULL) SceneObjects.Cross = new bartovra::Object(glm::vec3(0.15f, 0.10f, -0.73f),90.0f,0.05f,SceneState.elapsedTime);
  
  if(SceneObjects.Base == NULL) SceneObjects.Base = new bartovra::Object(glm::vec3(0.15f, 0.05f, 0.0f),90.0f,5.0f,SceneState.elapsedTime);
  
  if(SceneObjects.Pig == NULL) SceneObjects.Pig = new bartovra::Object(glm::vec3(0.0, 0.125f, 2.0f),-90.0f,0.1f,SceneState.elapsedTime);
  if(SceneObjects.Pig2 == NULL) SceneObjects.Pig2 = new bartovra::Object(glm::vec3(0.0, 0.125f, 1.4f),0.0f,0.06f,SceneState.elapsedTime);
  if(SceneObjects.Bat == NULL) SceneObjects.Bat = new bartovra::Object(glm::vec3(-0.05f, 1.0f, 1.2f),0.0f,0.1f,SceneState.elapsedTime);

  if(SceneObjects.Smoke1 == NULL) SceneObjects.Smoke1 = new bartovra::AniTex(glm::vec3(1.1f, 0.72f, 0.18f),0.1f,9,0.2f,SceneState.elapsedTime);//pub2
  if(SceneObjects.Smoke2 == NULL) SceneObjects.Smoke2 = new bartovra::AniTex(glm::vec3(-1.15f, 0.72f, 0.85f),0.1f,9,0.2f,SceneState.elapsedTime);//pub
  if(SceneObjects.Smoke3 == NULL) SceneObjects.Smoke3 = new bartovra::AniTex(glm::vec3(1.35f, 0.98f, 1.40f),0.1f,9,0.2f,SceneState.elapsedTime);//podloubi

  if(SceneObjects.Cloud == NULL) SceneObjects.Cloud = new bartovra::MovTex(glm::vec3(0.15f, 5.00f, -5.5f),4.0f,SceneState.elapsedTime);

}

glm::vec3 getCurrentDirection()
{
	glm::mat4 angleTransform =
		glm::rotate(glm::mat4(1.0f), camera.angleX, glm::vec3(0.0f, 0.0f, 0.0f)) * // x
		glm::rotate(glm::mat4(1.0f), camera.angleY, glm::vec3(1.0f, 0.0f, 0.0f)); // y

	return glm::vec3(camera.angleTransform * glm::vec4(camera.direction, 0.0f));

}

// First person view only
void checkSceneBorders(){

	//printf("X= %f,Y= %f, Z= %f",camera.position.x,camera.position.y,camera.position.z);

	if (camera.position.x<-2.5f){camera.position.x=-2.5f;printf("Collision with X\n");}
	else if (camera.position.x>2.5f){camera.position.x=2.5f;printf("Collision with X\n");}

	if (camera.position.z<-3.0f){camera.position.z=-3.0f;printf("Collision with Z\n");}
	else if (camera.position.z>3.0f){camera.position.z=3.0f;printf("Collision with Z\n");}
}

bool checkObjectCollision(){

	// fountain
	float radius=SceneObjects.Fountain->getRadius();
	float d_x=glm::abs(camera.position.x-SceneObjects.Fountain->getPosition().x);
	float d_y=glm::abs(camera.position.y-SceneObjects.Fountain->getPosition().y);
	float d_z=glm::abs(camera.position.z-SceneObjects.Fountain->getPosition().z);

	//printf("d_X= %f,d_Y= %f, d_Z= %f, Radius= %f\n",d_x,d_y,d_z,radius);

	if ((d_x<radius)&&(d_y<radius)&&(d_z<radius)){
		printf("Collision with fountain\n");
		return true;
	}

	return false;
}

void firstPersonCamera(){
	  if (camera.move==true){
		  if(SceneState.keyMap[KEY_UP_ARROW] == true)
		  {
			  checkSceneBorders();
			  camera.position+=STEP*getCurrentDirection();
			  if(checkObjectCollision()==true){camera.position-=STEP*getCurrentDirection();}
			  camera.position.y=0.2f;
			  SceneObjects.Pig->setFirstViewPosition(camera.position);
			  SceneObjects.Pig->setFirstViewAngle(camera.angleX-90.0f);
		  }

		  if(SceneState.keyMap[KEY_DOWN_ARROW] == true)
		  {
			  checkSceneBorders();
			  camera.position-=STEP*getCurrentDirection();
			  if(checkObjectCollision()==true){camera.position+=STEP*getCurrentDirection();}
			  camera.position.y=0.2f;
			  SceneObjects.Pig->setFirstViewPosition(camera.position);
			  SceneObjects.Pig->setFirstViewAngle(camera.angleX-90.0f);
		  }

		  if(SceneState.keyMap[KEY_LEFT_ARROW] == true)
		  {
			  checkSceneBorders();
			  camera.position-=STEP*glm::cross(getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
			  if(checkObjectCollision()==true){camera.position+=STEP*glm::cross(getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f));}
			  SceneObjects.Pig->setFirstViewPosition(camera.position);
			  SceneObjects.Pig->setFirstViewAngle(camera.angleX+90.0f);
		  }

		  if(SceneState.keyMap[KEY_RIGHT_ARROW] == true)
		  {
			  checkSceneBorders();
			  camera.position+=STEP*glm::cross(getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
			  if(checkObjectCollision()==true){camera.position-=STEP*glm::cross(getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f));}
			  SceneObjects.Pig->setFirstViewPosition(camera.position);
			  SceneObjects.Pig->setFirstViewAngle(camera.angleX+90.0f);
		  }

	  }
}

void freeCamera(){

  if (camera.move==true){
	if(SceneState.keyMap[KEY_UP_ARROW] == true)
	{
		camera.position+=STEP * getCurrentDirection();
	}

	if(SceneState.keyMap[KEY_DOWN_ARROW] == true)
	{
		camera.position-=STEP * getCurrentDirection();
	}

	if(SceneState.keyMap[KEY_LEFT_ARROW] == true)
	{
		camera.position-=STEP* glm::cross(getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if(SceneState.keyMap[KEY_RIGHT_ARROW] == true)
	{
		camera.position+=STEP* glm::cross(getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
  }

}

void drawWindowContents() {

  // setup parallel projection
  camera.orthoProjectionMatrix = glm::ortho(
    -SCENE_WIDTH, SCENE_WIDTH,
    -SCENE_HEIGHT, SCENE_HEIGHT,
    -10.0f*SCENE_DEPTH, 10.0f*SCENE_DEPTH
  );

  camera.angleTransform =
		glm::rotate(glm::mat4(1.0f), camera.angleX, glm::vec3(0.0f, -1.0f, 0.0f)) * // x
		glm::rotate(glm::mat4(1.0f), camera.angleY, glm::vec3(1.0f, 0.0f, 0.0f)); // y				

	camera.viewMatrix=glm::lookAt(
		camera.position, // position
		camera.position + glm::vec3(camera.angleTransform * glm::vec4(camera.direction, 0.0f)), // center
		glm::vec3(camera.angleTransform * glm::vec4(camera.upVector, 0.0f)) // up vector
	);

  camera.projectionMatrix = glm::perspective(60.0f, SCENE_WIDTH / (float)SCENE_HEIGHT, 0.1f, 30.0f);

  if (camera.firstPersonMode==true)firstPersonCamera();
  else freeCamera();

  //Shader setup
  glUseProgram(shaderProgram.program);
  glUniform1f(shaderProgram.timeLocation, SceneState.elapsedTime);

  //Fog uniforms
  setFogUniforms(SceneState.fogOnOff,SceneState.fogType,SceneState.fogTime);

  //Lightuniforms
  setLightUniforms(SceneState.sunOnOff,SceneState.pointOnOff);


  SceneObjects.Pig->setDirectionY(0.2f);

  //uniforms for spotlight
  glUniform3fv(shaderProgram.reflectorPositionLocation, 1, glm::value_ptr(glm::vec3(0.15f, 0.10f, -0.73f)));
  glUniform3fv(shaderProgram.reflectorDirectionLocation, 1, glm::value_ptr(glm::vec3(0.15f, 0.10f, -1.73f)));
  glUseProgram(0);

  // draw objects
 
  if(camera.firstPersonMode==false){drawPig(SceneObjects.Pig,PICKING_PIG, camera.viewMatrix, camera.projectionMatrix);}
  drawPub(SceneObjects.Pub,PICKING_PUB_1, camera.viewMatrix, camera.projectionMatrix);
  drawPub(SceneObjects.Pub2,PICKING_PUB2, camera.viewMatrix, camera.projectionMatrix);
  drawPodloubi(SceneObjects.Podloubi,PICKING_PODLOUBI, camera.viewMatrix, camera.projectionMatrix);
  drawChapel(SceneObjects.Chapel,PICKING_CHAPEL, camera.viewMatrix, camera.projectionMatrix);
  drawFountain(SceneObjects.Fountain,PICKING_FOUNTAIN, camera.viewMatrix, camera.projectionMatrix);
  drawWindmill(SceneObjects.Windmill,PICKING_WINDMILL, camera.viewMatrix, camera.projectionMatrix);
  drawCross(SceneObjects.Cross,PICKING_CROSS, camera.viewMatrix, camera.projectionMatrix);
  drawBase(SceneObjects.Base,PICKING_BASE, camera.viewMatrix, camera.projectionMatrix);
  drawBat(SceneObjects.Bat,PICKING_BAT, camera.viewMatrix, camera.projectionMatrix);

  drawFence(SceneObjects.Fence1,PICKING_FENCE_1, camera.viewMatrix, camera.projectionMatrix);
  drawFence(SceneObjects.Fence2,PICKING_FENCE_2, camera.viewMatrix, camera.projectionMatrix);
  drawFence(SceneObjects.Fence3,PICKING_FENCE_3, camera.viewMatrix, camera.projectionMatrix);

  if(SceneState.fence4Draw==true) drawFence(SceneObjects.Fence4,PICKING_FENCE_4, camera.viewMatrix, camera.projectionMatrix);
  if(SceneState.fence5Draw==true) drawFence(SceneObjects.Fence5,PICKING_FENCE_5, camera.viewMatrix, camera.projectionMatrix);
  if(SceneState.fence6Draw==true) drawFence(SceneObjects.Fence6,PICKING_FENCE_6, camera.viewMatrix, camera.projectionMatrix);
  if(SceneState.fence7Draw==true) drawFence(SceneObjects.Fence7,PICKING_FENCE_7, camera.viewMatrix, camera.projectionMatrix);
  if(SceneState.fence8Draw==true) drawFence(SceneObjects.Fence8,PICKING_FENCE_8, camera.viewMatrix, camera.projectionMatrix);
  if(SceneState.fence9Draw==true) drawFence(SceneObjects.Fence9,PICKING_FENCE_9, camera.viewMatrix, camera.projectionMatrix);

  if(SceneState.pigCurveDraw==true)drawPigCurve(SceneObjects.Pig2,PICKING_PIG_CURVE, camera.viewMatrix, camera.projectionMatrix);
  
  //Default picking number for other objects
  glStencilFunc(GL_ALWAYS, PICKING_WORLD, -1);

  // draw PGRskybox
  PGRdrawSkybox(camera.viewMatrix, camera.projectionMatrix);

  //draw clouds
  if (SceneState.cloud_on_off==true)drawCloud(SceneObjects.Cloud,PICKING_CLOUD_1, camera.viewMatrix, camera.projectionMatrix);

  // draw smoke with depth test disabled
  if (SceneState.smoke_on_off==true){
  //glDisable(GL_DEPTH_TEST);
  drawSmoke(SceneObjects.Smoke1,PICKING_SMOKE_1, camera.viewMatrix, camera.projectionMatrix);
  drawSmoke(SceneObjects.Smoke2,PICKING_SMOKE_2, camera.viewMatrix, camera.projectionMatrix); 
  drawSmoke(SceneObjects.Smoke3,PICKING_SMOKE_3, camera.viewMatrix, camera.projectionMatrix);  
  //glEnable(GL_DEPTH_TEST);
  }
}

// Called to update the display. You should call glutSwapBuffers after all of your
// rendering to display what you rendered.
void displayCallback() {
  GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  mask |= GL_STENCIL_BUFFER_BIT;

  glClear(mask);

  drawWindowContents();

  glutSwapBuffers();
}

// Called whenever the window is resized. The new window size is given, in pixels.
// This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshapeCallback(int newWidth, int newHeight) {
	printf("reshapeCallback\n");
  SceneState.windowWidth = newWidth;
  SceneState.windowHeight = newHeight;

  glViewport(0, 0, (GLsizei) newWidth, (GLsizei) newHeight);

}

void updateObjects(float elapsedTime){

	// update BAT on circle
	float fLoopDuration = 25.0f;
	float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fCurrTimeThroughLoop = fmodf(elapsedTime, fLoopDuration);

	SceneObjects.Bat->setPosition(glm::vec3(
	cosf(fCurrTimeThroughLoop * fScale) * 2.0f,
	//SceneObjects.Bat->getPosition().y,
	sinf(fCurrTimeThroughLoop * fScale) * 0.8f+1.0f,
	sinf(fCurrTimeThroughLoop * fScale) * 2.7f
	));
	//SceneObjects

	// update SMOKE billboard
	SceneObjects.Smoke1->setCurrentTime(elapsedTime);
	SceneObjects.Smoke2->setCurrentTime(elapsedTime);
	SceneObjects.Smoke3->setCurrentTime(elapsedTime);

	// update PIG on Curve
	SceneObjects.Pig2->setCurrentTime(SceneState.elapsedTime);

	float curveParamT = SceneObjects.Pig2->getSpeed() * (SceneObjects.Pig2->getCurrentTime() - SceneObjects.Pig2->getStartTime());

	SceneObjects.Pig2->setPosition(SceneObjects.Pig2->getOrigPosition() + evaluateClosedCurve( curveData, curveSize, curveParamT ));
	SceneObjects.Pig2->setDirection(glm::normalize(evaluateClosedCurve_1stDerivative(curveData, curveSize, curveParamT)));
	SceneObjects.Pig2->setPositionY(0.13f);
	SceneObjects.Pig2->setDirectionX(0.2f);


	//Camera on curve
	if(SceneState.cameraType==CURVE_VIEW){

		float cameraCurveParamT = CAMERA_CURVE_SPEED * (SceneState.elapsedTime - SceneState.CameraCurveStartTime);
		camera.position= cameraCurveData[0] + evaluateClosedCurve( cameraCurveData, cameraCurveSize, cameraCurveParamT );
		//camera.direction=glm::normalize(evaluateClosedCurve_1stDerivative(cameraCurveData, cameraCurveSize, cameraCurveParamT));
		camera.direction=evaluateClosedCurve_1stDerivative(cameraCurveData, cameraCurveSize, cameraCurveParamT);


	}

	//CLOUD
	SceneObjects.Cloud->setCurrentTime(elapsedTime);
	
}

void timerCallback(int) {
	//printf("Time: %f\n",SceneState.elapsedTime);
	// update scene time
	SceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds

	// update objects in the scene
	updateObjects(SceneState.elapsedTime);

	glutTimerFunc(33, timerCallback, 0);

	glutPostRedisplay();

}


// Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void initializeApplication() {

  // initialize random seed
  srand ((unsigned int)time(NULL));

  // initialize OpenGL
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  useLighting = true;
  SceneState.sunOnOff=true;
  SceneState.pointOnOff=false;
  SceneState.fogTime=false;
  SceneState.cloud_on_off = false;//true=on

  // initialize shaders
  initializeShaderPrograms();
  // create geometry for all models used
  initializeModels();

  SceneObjects.Pub = NULL;
  SceneObjects.Pub2 = NULL;
  SceneObjects.Podloubi = NULL;
  SceneObjects.Chapel = NULL;
  SceneObjects.Fountain = NULL;
  SceneObjects.Windmill = NULL;
  SceneObjects.Fence1 = NULL;
  SceneObjects.Fence2 = NULL;
  SceneObjects.Fence3 = NULL;
  SceneObjects.Fence4 = NULL;
  SceneObjects.Fence5 = NULL;
  SceneObjects.Fence6 = NULL;
  SceneObjects.Fence7 = NULL;
  SceneObjects.Fence8 = NULL;
  SceneObjects.Fence9 = NULL;
  SceneObjects.Base = NULL;
  SceneObjects.Pig = NULL;
  SceneObjects.Pig2 = NULL;
  SceneObjects.Bat = NULL;


  SceneObjects.Smoke1 = NULL;
  SceneObjects.Smoke2 = NULL;
  SceneObjects.Smoke3 = NULL;

  SceneObjects.Cloud = NULL;

  SceneState.cameraType=WINDMILL_VIEW;

  restartScene();
}

void finalizeApplication(void) {

  cleanUpObjects();

  // delete buffers - space ship, asteroid, missile, ufo, banner, and explosion
  cleanupModels();

  // delete shaders
  cleanupShaderPrograms();
}


// Called whenever a key on the keyboard was pressed. The key is given by the ''key''
// parameter, which is in ASCII. It's often a good idea to have the escape key
// (ASCII value 27) call glutLeaveMainLoop() to exit the program.
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {
  
  switch(keyPressed) {
    case 27:
      glutLeaveMainLoop();
      break;
	case 'r': // Restart
	  printf("Key R pressed.\n");
	  restartScene();
      break;
	case 'm': // Move lock/unlock
      camera.move = !camera.move;
		printf("Key M pressed.\n");
      glutPostRedisplay();
      break;
	  
    default:
      printf("Unrecognized key pressed\n");
  }
}

void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY) {

  switch (specKeyPressed) {
    case GLUT_KEY_RIGHT:
      SceneState.keyMap[KEY_RIGHT_ARROW] = true;
	  printf("Key KEY_RIGHT_ARROW pressed.\n");
      break;
    case GLUT_KEY_LEFT:
      SceneState.keyMap[KEY_LEFT_ARROW] = true;
	  printf("Key KEY_LEFT_ARROW pressed.\n");
      break;
    case GLUT_KEY_UP:
      SceneState.keyMap[KEY_UP_ARROW] = true;
	  printf("Key KEY_UP_ARROW pressed.\n");
      break;
    case GLUT_KEY_DOWN:
      SceneState.keyMap[KEY_DOWN_ARROW] = true;
	  printf("Key KEY_DOWN_ARROW pressed.\n");
      break;

	case GLUT_KEY_F1:
		SceneState.cameraType=WINDMILL_VIEW;
		camera.move=false;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;
	case GLUT_KEY_F2:
		SceneState.cameraType=GRAVEYARD_VIEW;
		camera.move=false;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;	
	case GLUT_KEY_F3:
		SceneState.cameraType=WINDMILL_VIEW;
		camera.move=true;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;
	case GLUT_KEY_F4:
		SceneState.cameraType=FREE_FIRST_PERSON_CAMERA;
		camera.move=true;
		camera.firstPersonMode = true;
		reDefineCamera();
		break;
	case GLUT_KEY_F5:
		SceneState.cameraType=CURVE_VIEW;
		SceneState.CameraCurveStartTime=SceneState.elapsedTime;
		camera.move=false;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;

    default:
      printf("Unrecognized special key pressed\n");
  }
}

void specialKeyboardUnpressedCallback(int specKeyPressed, int mouseX, int mouseY) {
	switch (specKeyPressed) {
		case GLUT_KEY_RIGHT:
			SceneState.keyMap[KEY_RIGHT_ARROW] = false;
			break;
		case GLUT_KEY_LEFT:
			SceneState.keyMap[KEY_LEFT_ARROW] = false;
			break;
		case GLUT_KEY_UP:
			SceneState.keyMap[KEY_UP_ARROW] = false;
			break;
		case GLUT_KEY_DOWN:
			SceneState.keyMap[KEY_DOWN_ARROW] = false;
			break;

	}
}

void menuHandler(int option)
{
	std::cout << "Menu Handler called" << std::endl;
	switch(option)
	{
	case WINDMILL_VIEW:
		SceneState.cameraType=WINDMILL_VIEW;
		camera.move=false;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;

	case GRAVEYARD_VIEW:
		SceneState.cameraType=GRAVEYARD_VIEW;
		camera.move=false;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;

	case FREE_CAMERA:
		SceneState.cameraType=WINDMILL_VIEW;
		camera.move=true;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;

	case FREE_FIRST_PERSON_CAMERA:
		SceneState.cameraType=FREE_FIRST_PERSON_CAMERA;
		camera.move=true;
		camera.firstPersonMode = true;
		reDefineCamera();
		break;

	case CURVE_VIEW:
		SceneState.cameraType=CURVE_VIEW;
		SceneState.CameraCurveStartTime=SceneState.elapsedTime;
		camera.move=false;
		camera.firstPersonMode = false;
		reDefineCamera();
		break;

	case SMOKE_ON_OFF:
		SceneState.smoke_on_off=!SceneState.smoke_on_off;
		break;

	case FOG_ON_OFF:
		if(SceneState.fogOnOff==0){SceneState.fogOnOff=1;SceneState.smoke_on_off=false;}
		else {SceneState.fogOnOff=0;SceneState.smoke_on_off=true;}
		break;

	case FOG_LINEAR:
		if(SceneState.fogOnOff==0)SceneState.fogOnOff=1;
		SceneState.fogType=1;
		if (SceneState.fogOnOff==1)SceneState.smoke_on_off=false;
		else SceneState.smoke_on_off=true;
		break;

	case FOG_EXPONENTIAL:
		if(SceneState.fogOnOff==0)SceneState.fogOnOff=1;
		SceneState.fogType=2;
		if (SceneState.fogOnOff==1)SceneState.smoke_on_off=false;
		else SceneState.smoke_on_off=true;
		break;

	case FOG_TIME:
		SceneState.fogTime=!SceneState.fogTime;
		if (SceneState.fogTime==true)SceneState.smoke_on_off=false;
		else SceneState.smoke_on_off=true;
		break;

	case SUN_ON_OFF:
		SceneState.sunOnOff=!SceneState.sunOnOff;
		break;

	case POINT_ON_OFF:
		SceneState.pointOnOff=!SceneState.pointOnOff;
		break;

	case CLOUD_ON_OFF:
		SceneState.cloud_on_off=!SceneState.cloud_on_off;
		break;

	}
}

void createMenu()
{
	int staticView = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Windmill", WINDMILL_VIEW);
	glutAddMenuEntry("Graveyard", GRAVEYARD_VIEW);

	int dynamicView = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Free camera", FREE_CAMERA);
	glutAddMenuEntry("First person camera", FREE_FIRST_PERSON_CAMERA);
	glutAddMenuEntry("Curve camera", CURVE_VIEW);

	int smoke = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Turn on/off", SMOKE_ON_OFF);

	int fog = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Turn on/off", FOG_ON_OFF);
	glutAddMenuEntry("Linear", FOG_LINEAR);
	glutAddMenuEntry("Exponential", FOG_EXPONENTIAL);
	glutAddMenuEntry("Time on/off", FOG_TIME);

	int light = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Turn sun light on/off", SUN_ON_OFF);
	glutAddMenuEntry("Turn point light on/off", POINT_ON_OFF);
	glutAddMenuEntry("Turn cloud on/off", CLOUD_ON_OFF);

	glutCreateMenu(menuHandler);
	glutAddSubMenu("Static camera", staticView);
	glutAddSubMenu("Dynamic camera", dynamicView);
	glutAddSubMenu("Smoke", smoke);
	glutAddSubMenu("Fog", fog);
	glutAddSubMenu("Light", light);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void doPicking(int button, int x, int y)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	unsigned char pixelID;

	drawWindowContents();

	glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &pixelID);
	glDisable(GL_STENCIL_TEST);


	if(button == GLUT_LEFT_BUTTON)
	
		switch(pixelID)
	{
		case PICKING_FENCE_1:
			SceneObjects.Fence1->setPosition(SceneObjects.Fence1->getPosition()+glm::vec3(0.0f,0.0f,0.1f));
			break;
		case PICKING_FENCE_2:
			SceneObjects.Fence2->setPosition(SceneObjects.Fence2->getPosition()+glm::vec3(0.0f,0.0f,0.1f));
			break;
		case PICKING_FENCE_3:
			SceneObjects.Fence3->setPosition(SceneObjects.Fence3->getPosition()+glm::vec3(0.0f,0.0f,0.1f));
			break;
		case PICKING_FENCE_4:
			SceneState.fence4Draw=false;
			break;
		case PICKING_FENCE_5:
			SceneState.fence5Draw=false;
			break;
		case PICKING_FENCE_6:
			SceneState.fence6Draw=false;
			break;
		case PICKING_FENCE_7:
			SceneState.fence7Draw=false;
			break;
		case PICKING_FENCE_8:
			SceneState.fence8Draw=false;
			break;
		case PICKING_FENCE_9:
			SceneState.fence9Draw=false;
			break;


		case PICKING_PIG_CURVE:
			SceneState.pigCurveDraw=false;
			break;

		case PICKING_SMOKE_1:
			SceneState.smoke_on_off=!SceneState.smoke_on_off;
			break;
		case PICKING_SMOKE_2:
			SceneState.smoke_on_off=!SceneState.smoke_on_off;
			break;
		case PICKING_SMOKE_3:
			SceneState.smoke_on_off=!SceneState.smoke_on_off;
			break;

		case PICKING_WORLD:
			printf("------Default picking number\n");
			break;
	}
}

// Mouse clicking
void onMouseCallback(int button, int state, int x, int y)
{
	int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	//if (camera.firstPersonMode==true){int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 4;}
	int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN) doPicking(button, x, glutGet(GLUT_WINDOW_HEIGHT) - 1 - y);
		break;
	}
	glutPostRedisplay();

}

//Mouse passive move
void onMousePassiveCallback(int mouseX, int mouseY)
{
	if (camera.move == false) {
		return;
	}

	bool needRedisplay = false;
	if ((SceneState.windowWidth/2)!=mouseX) {
		float angleXdelta = (float) mouseX - (float) SceneState.windowWidth/2;
		camera.angleX+=angleXdelta * 0.2f;
		needRedisplay=true;
	}

	if ((SceneState.windowHeight/2)!=mouseY) {
		float angleYdelta = (float) mouseY - (float) SceneState.windowHeight/2;
		camera.angleY+=angleYdelta * 0.2f;
		needRedisplay=true;
	}

	if (needRedisplay==true) {
		glutWarpPointer(SceneState.windowWidth/2, SceneState.windowHeight/2);
		glutPostRedisplay();
	}
	glutPostRedisplay();
}


int main(int argc, char** argv) {

  // initialize windowing system
  glutInit(&argc, argv);

  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

  // initial window size
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow(WINDOW_TITLE);


  glutDisplayFunc(displayCallback);

  // callback for change of window size
  glutReshapeFunc(reshapeCallback);

  // callbacks for keyboard
  glutKeyboardFunc(keyboardCallback);
  glutSpecialFunc(specialKeyboardCallback);// key pressed
  glutSpecialUpFunc(specialKeyboardUnpressedCallback);// key unpressed

  //callbacks for mouse
  glutPassiveMotionFunc(onMousePassiveCallback);
  glutMouseFunc(onMouseCallback);

  //callbacks for time
  glutTimerFunc(33, timerCallback, 0);

  //Create menu
  createMenu();

  // initialize GL, devil etc.
  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");

  initializeApplication();

  glutCloseFunc(finalizeApplication);

  glutMainLoop();

  return 0;
}