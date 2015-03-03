#ifndef _SEEK_H_
#define _SEEK_H_

#include "SteeringBehaviour.h"

class Seek : public SteeringBehaviour
{
public:
	AITank* target;
	bool mIsTagged = false;

	glm::vec2 GetForce();
};
#endif