#pragma once
#include "glm.hpp"

namespace Game
{
struct DeadReck
{
	DeadReck(float _serverDeltaTime);
	~DeadReck();

	struct Body
	{
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::quat orientation;
	};

	float serverDeltaTime;
	float timeSinceLastUpdate;
	uint64 timeStamp;
	Body clientStart;
	Body serverStart;

	void SetServerData(const Body& newServerData, bool hardReset, uint64 _timeStamp);
	Body Interpolate(float deltaTime);
};
}