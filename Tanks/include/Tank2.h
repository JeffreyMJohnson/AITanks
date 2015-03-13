#ifndef _TANK_2_H_
#define _TANK_2_H_
#include "Globals.h"
#include "Prey.h"

class Tank2 : public Prey
{
public:
	Tank2(Framework* framework, glm::vec2 position);

	void Update();

	void SetHunter(IBoid& hunter);
	void SetTarget(IBoid& target);
private:
	IBoid* mHunter = nullptr;
	IBoid* mTarget = nullptr;
};

#endif