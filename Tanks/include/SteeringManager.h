#include "IBoid.h"
#include "Globals.h"

typedef glm::vec2 vec;


class SteeringManager
{
public:
	vec mSteering;
	IBoid* mHost;

	SteeringManager(IBoid* host);

	void Seek(vec target, float slowingRadius);
	void Flee(vec target);
	void Wander();
	void Evade(IBoid& target);
	void Pursuit(IBoid& target);

	void Update();

	void Reset();

private:
	//MAGIC NUMBERS 
	const float CIRCLE_DISTANCE = 50;
	const float CIRCLE_RADIUS = 100;
	const int ANGLE_CHANGE = 10;
	const float WANDER_FORCE_SCALE = .5f;
	
	float mWanderAngle_ = 0.0f;
	
	vec DoSeek(vec target, float slowingRadius);
	vec DoFlee(vec target);
	vec DoWander();
	vec DoEvade(IBoid& target);
	vec DoPursuit(IBoid& target);
	void SetAngle(vec& vector, float value);

	void Truncate(vec& original, float max);
}; 