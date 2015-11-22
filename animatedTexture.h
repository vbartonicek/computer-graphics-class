// -----------------------------------------------------------
/**
 * \file animatedTexture.h
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief Definitions of animated texture functions
*/
// ----------------------------------------------------------


#pragma once
#include <iostream>
#include "pgr.h"
#include "data.h"

namespace bartovra{

class AniTex
{
public:
	AniTex(glm::vec3 position,float size,int frames,float frameDuration,float startTime){this->position=position;this->Origposition=this->position;this->size=size;this->textureFrames=frames;this->frameDuration=frameDuration;this->startTime=startTime;this->currentTime=this->startTime;this->speed=0.0f;this->frameCurrent=1;this->printValues();};
	~AniTex();
	glm::vec3 getPosition(){return this->position;};
	float getSize(){return this->size;};
	float getStartTime(){return this->startTime;};
	float getCurrentTime(){return this->currentTime;};
	float getSpeed(){return this->speed;};
	int getTextureFrames(){return this->textureFrames;}
	float getFrameDuration(){return this->frameDuration;};
	int getCurrentFrame(){return this->frameCurrent;};

	void setPosition(glm::vec3 newPosition){this->position=newPosition;}
	void setOrigPosition(){this->position=this->Origposition;}
	void setStartTime(float newTime){this->startTime=newTime;}
	void setCurrentTime(float newTime){this->currentTime=newTime;}
	void setCurrentFrame(){if(this->frameCurrent<this->textureFrames)this->frameCurrent++;else this->frameCurrent=1;}
	void printValues(){printf("startTime=%f, currentTime=%f, textureFrames=%d, frameDuration=%f/n",startTime,currentTime,textureFrames,frameDuration);}
private:

	glm::vec3 position;
	glm::vec3 Origposition;
	float     size;
	float startTime;
	float currentTime;
	float speed;

	int    textureFrames;
	int	   frameCurrent;
	float  frameDuration;
};

}