#ifndef _STATE_TANK_H_
#define _STATE_TANK_H_

#include "Tank.h"
#include "Grid.h"


class StateTank : public Tank
{
public:

	void Initialize(Framework* framework, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid);

	void Update(float timeDelta);
	void Draw();

	vec FindClosestBase();
	vec FindClosestResource();

private:
	Grid* mGrid;
};

#endif