#ifndef	_TAG_TANK_H_
#define _TAG_TANK_H_

#include "Entity.h"
#include "SteeringManager.h"
#include "IBoid.h"


/*
This class has steering logic for playing tag with another TagTank object.
*/
class TagTank : 
	public Entity,
	public IBoid
{
public:
	glm::vec2 mVelocity;
	float mMass = 1.0f;
	float mMaxVelocity = 1.0f;
	SteeringManager* mSteering;
	TagTank* tagPartner;
	bool isSeeking;
	glm::vec4 mBounds;
	

	~TagTank();

	void Initialize(Framework* framework, TagTank* tagPartner, bool isSeeking);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, TagTank* tagPartner, bool isSeeking);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, TagTank* tagPartner, bool isSeeking);
	//IBoid interface methods
	glm::vec2& GetVelocity();
	float GetMaxVelocity();
	glm::vec2& GetPosition();
	float GetMass();

	//Entity interface methods
	void Update(float timeDelta);
	void Draw();
private:

};

#endif