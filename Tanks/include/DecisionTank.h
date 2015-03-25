#ifndef _DECISION_TANK_H_
#define _DECISION_TANK_H_

#include "Tank.h"
#include "SteeringManager.h"
#include "Decision.h"

class DecisionTank :
	public Tank
{
public:
	Decision* mDecisionTree = nullptr;
	float mTimer = 0.0f;

	//Entity interface methods
	void Update(float timeDelta);
	void Draw();
		
};


#endif