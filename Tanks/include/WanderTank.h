#ifndef _WANDER_TANK_H_
#define _WANDER_TANK_H_
#include "Tank.h"

class WanderTank :
	public Tank
{
public:
	//Entity interface methods
	void Update(float timeDelta);
	void Draw();
};


#endif