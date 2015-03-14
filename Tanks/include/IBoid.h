#ifndef _I_BOLD_H_
#define _I_BOLD_H_
#define GLM_FORCE_PURE
#include "glm\glm.hpp"

class IBoid
{
public:
	virtual glm::vec2& GetVelocity() = 0;
	virtual float GetMaxVelocity() = 0;
	virtual glm::vec2& GetPosition() = 0;
	virtual float GetMass()=0;
};

#endif