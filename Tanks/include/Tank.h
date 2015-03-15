#ifndef _TANK_H_
#define _TANK_H_

#include "Entity.h"
#include "SteeringManager.h"
#include "IBoid.h"

typedef MNF::Collider::AABB AABB;

class Tank :
	public Entity,
	public IBoid
{
public:
	glm::vec2 mVelocity;
	float mMass = 1.0f;
	float mMaxVelocity = 1.0f;
	float mRotationAngle = 0.0f;
	glm::vec4 mBounds;

	~Tank();

	//IBoid interface methods
	glm::vec2& GetVelocity();
	float GetMaxVelocity();
	glm::vec2& GetPosition();
	float GetMass();

	//Entity interface methods
	void Update(float timeDelta){};
	void Draw(){};

protected:
	SteeringManager* mSteering;


};

#endif