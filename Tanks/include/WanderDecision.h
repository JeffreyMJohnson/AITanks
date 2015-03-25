#ifndef _WANDER_DECISION_H_
#define _WANDER_DECISION_H_
#include "Decision.h"

#include "DecisionTank.h"

class WanderDecision :
	public Decision
{
public:
	WanderDecision(DecisionTank* owner);
	void MakeDecision();
};

#endif