#ifndef _PREY_H_
#define _PREY_H_

#include "framework\Framework.h"
#include "IBoid.h"
#include "SteeringManager.h"

typedef glm::vec2 vec;

class Prey : public IBoid
{
public:
	Framework* mFramework;
	unsigned int mspriteID;
	glm::vec4 mColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec mPosition;
	vec mVelocity;
	SteeringManager* mSteering;

	Prey(Framework* framework, vec position);
	~Prey();

	void Update();
	void Draw();

	//IBoid interface required methods
	glm::vec2& GetVelocity();
	float GetMaxVelocity();
	glm::vec2& GetPosition();
	float GetMass();


private:
	//MAGIC NUMBERS
	const float WIDTH = 20.0f;
	const float HEIGHT = 20.0f;
	const float MASS = 1.0f;
	const float MAX_VELOCITY = 1.0f;

};

#endif