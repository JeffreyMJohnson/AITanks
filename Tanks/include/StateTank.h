#ifndef _STATE_TANK_H_
#define _STATE_TANK_H_

#include "Tank.h"
#include "Grid.h"

#include <vector>
#include <list>
#include <limits>

class StateManager;

enum HEURISTIC_TYPE
{
	DISTANCE,
	MANHATTAN,
	DIAGONAL
};

class StateTank : public Tank
{
public:

	static unsigned int mTotalResourceQuantity;//uninitialized because static
	float mCollectionSpeed = .5f;
	float mCollectionTimer = 0.0f;
	unsigned int mTotalResourcesAllowed = 10;
	unsigned int mCurrentResourcesQuantity = 0;
	std::vector<Tile*> mPathTileList;
	Grid* mGrid;

	void Initialize(Framework* framework, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid);

	void Update(float timeDelta);
	void Draw();
	unsigned int GetCurrentResourceQty();
	Tile* FindClosestBase();
	Tile* FindClosestResource();
	void AStarPathFind(Tile* goal, bool smoothPath = false);
	void DkPathFind(Tile* goal);

	void UpdateRotation();


private:

	//STATE mCurrentState;
	StateManager* mStateManager;



	Tile* mBaseTile;


	float GetHeuristic(HEURISTIC_TYPE type, Tile* node, Tile* nodeTarget);
	bool HasStraightLine(Tile* start, Tile* goal);
	Tile* FindClosest(const std::vector<Tile*>& tileList);
};


#endif