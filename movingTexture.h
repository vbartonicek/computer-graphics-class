// -----------------------------------------------------------
/**
 * \file movingTexture.h
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief Definitions of moving texture functions
*/
// ----------------------------------------------------------

#pragma once
#include <iostream>
#include "pgr.h"
#include "data.h"

namespace bartovra{

class MovTex
{
public:
	MovTex(glm::vec3 position,float size,float startTime){this->position=position;this->size=size;this->startTime=startTime;};
	~MovTex();
	glm::vec3 getPosition(){return this->position;};
	float getSize(){return this->size;};
	float getStartTime(){return this->startTime;};
	float getCurrentTime(){return this->currentTime;};

	void setPosition(glm::vec3 newPosition){this->position=newPosition;}
	void setStartTime(float newTime){this->startTime=newTime;}
	void setCurrentTime(float newTime){this->currentTime=newTime;}
private:

	glm::vec3 position;
	float     size;
	float startTime;
	float currentTime;
};
}