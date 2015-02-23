#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "glm\glm.hpp"

namespace MNF
{
	class Globals
	{
	public:
		static const int SCREEN_WIDTH = 1024;
		static const int SCREEN_HEIGHT = 768;


	};

	class Collider
	{
	public:
		static bool CircleCircle(const glm::vec2& circle1Position, const float& circle1Radius, const glm::vec2& circle2Position, const float& circle2Radius)
		{
			float distance = glm::distance(circle1Position, circle2Position);
			if (distance < (circle1Radius + circle2Radius))
			{
				return true;
			}
			return false;
		}

		static bool AABB(const glm::vec2& box1Min, const glm::vec2& box1Max, const glm::vec2& box2Min, const glm::vec2& box2Max)
		{
			if (box1Min.x > box2Max.x || box2Min.x > box1Max.x)
				return false;
			if (box1Min.y > box2Max.y || box2Min.y > box1Max.y)
				return false;
			return true;
		}
	};

	class Utils
	{
	public:
		static bool Equalsf(float lhs, float rhs, float delta)
		{
			return (fabs(lhs - rhs) < delta);
		}
	};
}

#endif