#include "config.h"
#include "dead_reck.h"

namespace Game
{

DeadReck::DeadReck(float _serverDeltaTime)
{
	serverDeltaTime = _serverDeltaTime;
	timeSinceLastUpdate = 0.f;
	timeStamp = 0;
	clientStart = { glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f), glm::identity<glm::quat>() };
	serverStart = {glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f), glm::identity<glm::quat>()};
}

DeadReck::~DeadReck() {} //empty

void DeadReck::SetServerData(const Body& newServerData, bool hardReset, uint64 _timeStamp)
{
	if (_timeStamp < timeStamp)
		return;

	timeStamp = _timeStamp;

	if (hardReset)
	{
		clientStart = newServerData;
	}
	else
	{
		// set client start body to current interpolated body
		clientStart = Interpolate(0.f);
	}

	// reset the server body
	timeSinceLastUpdate = 0.f;
	serverStart = newServerData;
}

DeadReck::Body DeadReck::Interpolate(float deltaTime)
{
	timeSinceLastUpdate += deltaTime;
	timeSinceLastUpdate = timeSinceLastUpdate > serverDeltaTime ? serverDeltaTime : timeSinceLastUpdate;

	float normT = timeSinceLastUpdate / serverDeltaTime;

	//create interpolated blends
	glm::vec3 velBlend = clientStart.velocity + (serverStart.velocity - clientStart.velocity) * normT;
	glm::vec3 halfAt2 = serverStart.acceleration * (timeSinceLastUpdate * timeSinceLastUpdate);
	glm::vec3 posBlendClient = clientStart.position + velBlend * timeSinceLastUpdate + halfAt2;
	glm::vec3 posBlendServer = serverStart.position + serverStart.velocity * timeSinceLastUpdate + halfAt2;
	glm::vec3 posBlend = posBlendClient + (posBlendServer - posBlendClient) * normT;

	glm::quat dirBlend = glm::mix(clientStart.orientation, serverStart.orientation, normT);

	return {
		posBlend,
		velBlend,
		serverStart.acceleration,
		dirBlend
	};
}
}