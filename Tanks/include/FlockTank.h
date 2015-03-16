#ifndef _FLOCK_TANK_H_
#define _FLOCK_TANK_H_

#include "Tank.h"
#include <vector>

class FlockTank :
	public Tank
{
public:
	void Initialize(Framework* framework, std::vector<Entity*>& entityList);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, std::vector<Entity*>& entityList);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, std::vector<Entity*>& entityList);

	void Update(float timeDelta);
	void Draw();

private:
	std::vector<Entity*> mEntityList;
};



#endif