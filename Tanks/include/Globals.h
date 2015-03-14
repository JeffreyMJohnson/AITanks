#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#define GLM_FORCE_PURE
#include "glm\glm.hpp"

#define PI 3.14159265359f
#define RADIAN_CONVERSION PI / 180.0f
#define DEGREE_CONVERSION 180.0f / PI


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
		struct Ray
		{
			glm::vec2 origin;
			glm::vec2 direction;

			Ray(glm::vec2 origin, glm::vec2 direction)
			{
				this->origin = origin;
				this->direction = direction;
			}


		};

		struct AABB
		{
			glm::vec2 minPoint;
			glm::vec2 maxPoint;

			AABB(glm::vec2 minPoint, glm::vec2 maxPoint)
			{
				this->minPoint = minPoint;
				this->maxPoint = maxPoint;
			}
		};

		struct Plane
		{
			glm::vec2 point;
			glm::vec2 normal;

			Plane(glm::vec2 point, glm::vec2 normal)
			{
				this->point = point;
				this->normal = normal;
			}
		};


		static bool CircleCircle(const glm::vec2& circle1Position, const float& circle1Radius, const glm::vec2& circle2Position, const float& circle2Radius)
		{
			float distance = glm::distance(circle1Position, circle2Position);
			if (distance < (circle1Radius + circle2Radius))
			{
				return true;
			}
			return false;
		}

		static bool AABBCollide(const AABB& box1, const AABB& box2)
		{
			if (box1.minPoint.x > box2.maxPoint.x || box2.minPoint.x > box1.maxPoint.x)
			{
				return false;
			}
			if (box1.minPoint.y > box2.maxPoint.y || box2.minPoint.y > box1.maxPoint.y)
			{
				return false;
			}
			return true;
		}

		static bool RayAABBIntersect(Ray& ray, AABB& box, float& enter, float& exit)
		{
			glm::vec2 min = (box.minPoint - ray.origin) / ray.direction;
			glm::vec2 max = (box.maxPoint - ray.origin) / ray.direction;

			glm::vec2 near = glm::min(min, max);
			glm::vec2 far = glm::max(min, max);

			enter = glm::max(glm::max(near.x, near.y), 0.0f);
			exit = glm::min(far.x, far.y);

			return (exit > 0.0f && enter < exit);
		}

		static bool RayPlaneIntersect(Ray& ray, Plane& plane, float& t)
		{
			float denom = glm::dot(plane.normal, ray.direction);
			if (denom > 1e-6)
			{
				glm::vec2 point_origin = plane.point - ray.origin;
				t = glm::dot(point_origin, plane.normal) / denom;
				return (t >= 0);
			}
			return false;

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