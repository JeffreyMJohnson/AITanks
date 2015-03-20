#ifndef _STATE_TANK_H_
#define _STATE_TANK_H_

#include "Tank.h"
#include "Grid.h"


class StateManager;

class StateTank : public Tank
{
public:
	/*
	enum STATE
	{
		GO_TO_RESOURCE,
		COLLECT_RESOURCE,
		DEPOSIT_RESOURCE
	};
	*/
	
	static unsigned int mTotalResourceQuantity;
	float mCollectionSpeed = .5f;
	float mCollectionTimer = 0.0f;
	unsigned int mTotalResourcesAllowed = 10;
	unsigned int mCurrentResourcesQuantity = 0;

	void Initialize(Framework* framework, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid);

	void Update(float timeDelta);
	void Draw();
	unsigned int GetCurrentResourceQty();
	vec FindClosestBase();
	vec FindClosestResource();



private:
	Grid* mGrid;
	//STATE mCurrentState;
	StateManager* mStateManager;
	

	Tile* mBaseTile;
	Tile* mResourceTile;

	
};


#endif