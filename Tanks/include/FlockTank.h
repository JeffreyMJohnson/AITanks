#ifndef _FLOCK_TANK_H_
#define _FLOCK_TANK_H_

#include "Tank.h"
#include <vector>

class FlockTank :
	public Tank
{
public:
	std::vector<Entity*>* mEntityList;
	const float REPULSION_FORCE = 5.0f;
	const float COHESION_FORCE = 4.0f;
	const float NEIGHBOR_RADIUS = 35.0f;

	void Initialize(Framework* framework, std::vector<Entity*>* entityList);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, std::vector<Entity*>* entityList);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, std::vector<Entity*>* entityList);

	void Update(float timeDelta);
	void Draw();

private:
	
};



#endif