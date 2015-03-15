#ifndef	_TAG_TANK_H_
#define _TAG_TANK_H_

#include "Tank.h"
//#include "Entity.h"
//#include "SteeringManager.h"
//#include "IBoid.h"

typedef MNF::Collider::AABB AABB;

/*
This class has steering logic for playing tag with another TagTank object.
*/
class TagTank : 
	public Tank
{
public:
	
	TagTank* tagPartner;
	bool isSeeking;

	~TagTank();

	void Initialize(Framework* framework, TagTank* tagPartner, bool isSeeking);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, TagTank* tagPartner, bool isSeeking);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, TagTank* tagPartner, bool isSeeking);
	
	//Entity interface methods
	void Update(float timeDelta);
	void Draw();
private:
	bool mIsTagged = false;
	float mWaitTimer = 0.0f;

	const float SEEK_PAUSE_TIME = 5.0f;
	const float VISIBILITY_RADIUS = 100.0f;
};

#endif