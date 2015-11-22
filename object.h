// -----------------------------------------------------------
/**
 * \file object.h
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief Definitions of class Object
*/
// ----------------------------------------------------------
#pragma once
#include <iostream>
#include "pgr.h"
#include "data.h"

namespace bartovra{

class Object
{
	public:
		Object(glm::vec3 position,float viewAngle,float size,float startTime){this->position=position;this->origPosition=this->position;this->viewAngle=viewAngle;this->size=size;this->radius=0.2f;this->startTime=startTime;this->currentTime=this->startTime;this->speed=0.2f;};
		~Object();
		glm::vec3 getPosition(){return this->position;};
		glm::vec3 getOrigPosition(){return this->origPosition;};
		glm::vec3 getDirection(){return this->direction;};
		float getSize(){return this->size;};
		float getViewAngle(){return this->viewAngle;};
		float getRadius(){return this->radius;};
		float getStartTime(){return this->startTime;};
		float getCurrentTime(){return this->currentTime;};
		float getSpeed(){return this->speed;};

		void setPosition(glm::vec3 newPosition){this->position=newPosition;}
		void setPositionY(float newY){this->position.y=newY;}
		void setOrigPosition(){this->position=this->origPosition;}
		void setDirection(glm::vec3 newDirection){this->direction=newDirection;}
		void setDirectionX(float newX){this->direction.x=newX;}
		void setDirectionY(float newY){this->direction.y=newY;}
		void setDirectionZ(float newZ){this->direction.z=newZ;}
		void setFirstViewPosition(glm::vec3 newPosition){this->position=newPosition;this->position.y=FIRST_PERSON_Y;}
		void setFirstViewAngle(float newViewAngle){this->viewAngle=newViewAngle;}
		void setCurrentTime(float newTime){this->currentTime=newTime;}
	private:

		glm::vec3 position;
		glm::vec3 origPosition;
		glm::vec3 direction;
		float     size;
		float viewAngle; // in degrees
		float radius;
		float startTime;
		float currentTime;
		float speed;
};
}