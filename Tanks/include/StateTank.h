#ifndef _STATE_TANK_H_
#define _STATE_TANK_H_

#include "Tank.h"
#include "Grid.h"


class StateTank : public Tank
{
public:

	enum STATE
	{
		GO_TO_RESOURCE,
		COLLECT_RESOURCE,
		DEPOSIT_RESOURCE
	};

	void Initialize(Framework* framework, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid);

	void Update(float timeDelta);
	void Draw();

	unsigned int GetCurrentResourceQty();
	static unsigned int mTotalResourceQuantity;


private:
	Grid* mGrid;
	STATE mCurrentState;
	float mCollectionSpeed = .5f;
	float mCollectionTimer = 0.0f;
	unsigned int mTotalResourcesAllowed = 10;
	unsigned int mCurrentResourcesQuantity = 0;
	Tile* mBaseTile;
	Tile* mResourceTile;

	vec FindClosestBase();
	vec FindClosestResource();

	void RunGoToResource(float deltaTime);
	void RunCollectResources(float deltaTime);
	void RunDepositResources(float deltaTime);

	
};


#endif