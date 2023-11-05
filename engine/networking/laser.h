#pragma once
#include "render/model.h"

namespace Game
{
struct Laser
{
	const uint32 id;
	const uint64 startTime;
	const uint64 endTime;
	const glm::vec3 origin;
	const glm::quat direction;

	const uint32 spaceShipId;


	Laser(uint32 _id, uint64 _startTime, uint64 _endTime, const glm::vec3& _origin, const glm::quat& _direction, uint32 _spaceShipId);
	~Laser();

	float GetSecondsAlive(uint64 currentTimeMillis);
	glm::vec3 GetPosition(uint64 currentTimeMillis, float velocity);
	glm::vec3 GetDirection();
	glm::mat4 GetLocalToWorld(uint64 currentTimeMillis, float velocity);
};
}