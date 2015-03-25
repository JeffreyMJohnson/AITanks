#include "DecisionTank.h"


void DecisionTank::Update(float timeDelta)
{
	mTimer-= timeDelta;
	mDecisionTree->MakeDecision();
}

void DecisionTank::Draw()
{

}