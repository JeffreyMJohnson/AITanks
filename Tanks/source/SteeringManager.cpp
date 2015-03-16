#include "SteeringManager.h"

SteeringManager::SteeringManager(IBoid* host)
{
	mHost = host;
	mSteering = vec(0, 0);
}

void SteeringManager::Seek(vec target, float slowingRadius)
{
	mSteering += DoSeek(target, slowingRadius);
}

void SteeringManager::Flee(vec target)
{
	mSteering += DoFlee(target);
}

void SteeringManager::Wander()
{
	mSteering += DoWander();
}

void SteeringManager::Evade(IBoid& target)
{
	mSteering += DoEvade(target);
}

void SteeringManager::Pursuit(IBoid& target)
{
	mSteering += DoPursuit(target);
}

void SteeringManager::Separate()
{
	mSteering += DoSeparate();
}

void SteeringManager::Alignment(std::vector<IBoid*>& agentList)
{
	mSteering += DoAlignment(agentList);
}

void SteeringManager::Update()
{
	vec& velocity = mHost->GetVelocity();
	vec& position = mHost->GetPosition();

	Truncate(mSteering, mHost->GetMaxVelocity());
	mSteering *= (1 / mHost->GetMass());

	velocity += mSteering;
	Truncate(velocity, mHost->GetMaxVelocity());
	
	position += mHost->GetVelocity();
}

void SteeringManager::Reset()
{

}

/*
Returns vector2 with a magnitude no longer than given value.
*/
void SteeringManager::Truncate(vec& original, float max)
{
	if (glm::length(original) > max)
	{
		original = glm::normalize(original);
		original *= max;
	}
}



vec SteeringManager::DoSeek(vec target, float slowingRadius = 0.0f)
{
	vec force = vec(0, 0);
	float distance = 0.0f;

	vec desired = target - mHost->GetPosition();
	distance = glm::length(desired);
	desired = glm::normalize(desired);

	if (distance <= slowingRadius)
	{
		desired *= (mHost->GetMaxVelocity() * distance / slowingRadius);
	}
	else
	{
		desired *= mHost->GetMaxVelocity();
	}

	force = desired - mHost->GetVelocity();

	return force;
}

vec SteeringManager::DoFlee(vec target)
{
	return -DoSeek(target);
}

vec SteeringManager::DoWander()
{
	//CIRCLE CENTER POSITION
	//calculate center of circle, because it's in front of owner, use the velocity as a guide.
	vec circleCenter = mHost->GetVelocity();
	//take care od edge case velocity = 0;
	if (circleCenter == vec(0, 0))
	{
		//50/50 chance of being positive or negative
		float x = (rand() % 11) - 5.0f;
		float y = (rand() % 11) - 5.0f;
		circleCenter = glm::normalize(vec(x, y)) * mHost->GetMaxVelocity();
	}

	//normalize into vector with only direction
	circleCenter = glm::normalize(circleCenter);
	//scale vector by the distance the circle is from owner for correct magnitude.
	circleCenter *= CIRCLE_DISTANCE;

	//DISPLACEMENT FORCE - responsible for left/right turn
	//use vector aligned with y axis
	vec displacemenet(0, -1);
	//scale it by circle radius
	displacemenet *= CIRCLE_RADIUS;

	//randomly change vector direction by making it change it's current angle.
	SetAngle(displacemenet, (float)mWanderAngle_);

	//change wander angle a bit, so won't have same value next frame
	/*adding 1 to angleChange for modulas allows the variable to be included in potential
	random number as well as 0.  This keeps the distribution 50/50 when subtracting by half,
	otherwise the result is weighted to the below-half side and the tank will tend to circle in
	one direction.*/

	int r = (rand() % (ANGLE_CHANGE + 1));
	int s = (int)(ANGLE_CHANGE * .5f);
	//subtracting by half changes scale around 0, equally distributed neg/pos.
	mWanderAngle_ += r - s;

	//RETURN FORCE
	//scale up because final result is scaled down by deltaTime
	vec force = (circleCenter + displacemenet) * WANDER_FORCE_SCALE;
	return force;


}

vec SteeringManager::DoEvade(IBoid& target)
{
	//float distance = glm::distance(target.GetPosition(), mHost->GetPosition());
	vec futurePosition = target.GetPosition() + target.GetVelocity();

	return DoFlee(futurePosition);
}

vec SteeringManager::DoPursuit(IBoid& target)
{
	//float distance = glm::distance(target.GetPosition(), mHost->GetPosition());
	vec targetFuturePosition = target.GetPosition() + target.GetVelocity();

	return DoSeek(targetFuturePosition);
}

vec SteeringManager::DoSeparate()
{
	return vec();
}

vec SteeringManager::DoAlignment(std::vector<IBoid*>& agentList)
{
	vec force;
	int neighborCount = 0;

	for (auto agent : agentList)
	{
		if (glm::distance(mHost->GetPosition(), agent->GetPosition()) > 300)
		{
			force += agent->GetVelocity();
			neighborCount++;
		}
	}

	return vec();
}

void SteeringManager::SetAngle(glm::vec2& vector, float value)
{
	float length = glm::length(vector);
	vector.x = cosf(value * RADIAN_CONVERSION) * length;
	vector.y = sinf(value * RADIAN_CONVERSION) * length;
}