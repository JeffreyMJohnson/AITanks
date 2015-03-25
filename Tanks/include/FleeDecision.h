#ifndef _FLEE_DECISION_H_
#define _FLEE_DECISION_H_
#include "Decision.h"
#include "DecisionTank.h"

class FleeDecision :
	public Decision
{
public:
	glm::vec2 mTarget;

	FleeDecision(DecisionTank* owner, glm::vec2& target);

	void MakeDecision();
};


#endif