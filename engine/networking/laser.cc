#include "config.h"
#include "laser.h"
#include "render/physics.h"

namespace Game
{
	Laser::Laser(uint32 _id, uint64 _startTime, uint64 _endTime, const glm::vec3& _origin, const glm::quat& _direction, uint32 _spaceShipId) :
	id(_id),
	startTime(_startTime),
	endTime(_endTime),
	origin(_origin),
	direction(_direction),	
	spaceShipId(_spaceShipId)
{}

Laser::~Laser() {}

float Laser::GetSecondsAlive(uint64 currentTimeMillis)
{
	return 0.001f * static_cast<float>(currentTimeMillis - startTime);
}

glm::vec3 Laser::GetPosition(uint64 currentTimeMillis, float velocity)
{
	return origin + direction * glm::vec3(0.f, 0.f, GetSecondsAlive(currentTimeMillis) * velocity);
}

glm::vec3 Laser::GetDirection()
{
	return direction * glm::vec3(0.f, 0.f, 1.f);
}

glm::mat4 Laser::GetLocalToWorld(uint64 currentTimeMillis, float velocity)
{
	glm::vec3 pos = GetPosition(currentTimeMillis, velocity);
	return glm::translate(pos) * (glm::mat4)direction;
}
}