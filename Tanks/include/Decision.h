#ifndef _DECISION_H_
#define _DECISION_H_

#include "Tank.h"

class Decision
{
public:
	Tank* mOwner;

	virtual void MakeDecision() = 0;
};
#endif