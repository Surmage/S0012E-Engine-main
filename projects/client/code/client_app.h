#pragma once

#include "core/app.h"
#include "render/window.h"
#include "render/model.h"
#include "render/physics.h"
#include "networking/console.h"
#include "networking/network.h"
#include "networking/spaceship.h"
#include "networking/laser.h"
#include <vector>
#include "..\..\generated\flat\proto.h"

class ClientApp : public Core::App
{
public:
	ClientApp();
	~ClientApp();

	bool Open();
	void Run();
	void Exit();

	void OnServerConnect();
	void OnServerDisconnect();

private:
	// updates
	void RenderUI();
	void UpdateNetwork();
	void GetControlledSpaceShip();
	void UpdateSpaceShips(float deltaTime);
	void UpdateLasers();

	// unpack messages from server
	void UnpackPlayer(const Protocol::Player* player, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration, glm::quat& orientation, uint32& id);
	void UnpackLaser(const Protocol::Laser* laser, glm::vec3& origin, glm::quat& direction, uint64& spawnTime, uint64& despawnTime, uint32& id);
	void HandleMsgClientConnect(const Protocol::PacketWrapper* packet);
	void HandleMsgGameState(const Protocol::PacketWrapper* packet);
	void HandleMsgSpawnPlayer(const Protocol::PacketWrapper* packet);
	void HandleMsgDespawnPlayer(const Protocol::PacketWrapper* packet);
	void HandleMsgUpdatePlayer(const Protocol::PacketWrapper* packet);
	void HandleMsgTeleportPlayer(const Protocol::PacketWrapper* packet);
	void HandleMsgSpawnLaser(const Protocol::PacketWrapper* packet);
	void HandleMsgDespawnLaser(const Protocol::PacketWrapper* packet);
	void HandleMsgText(const Protocol::PacketWrapper* packet);

	// utility functions
	unsigned short CompressInputData(const Game::Input& data);
	Game::Input GetInputData();
	size_t SpaceShipIndex(uint32 spaceShipId);
	size_t LaserIndex(uint32 laserId);

	// methods in response to server messages
	
	void SpawnSpaceShip(const glm::vec3& position, uint32 spaceShipId);
	void DespawnSpaceShip(uint32 spaceShipId);
	void UpdateSpaceShipData(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration, const glm::quat& direction, uint32 spaceShipId, bool hardReset, uint64 timeStamp);
	void SpawnLaser(const glm::vec3& origin, const glm::quat& direction, uint32 spaceShipId, uint64 spawnTime, uint64 despawnTime, uint32 laserId);
	void DespawnLaser(uint32 laserId);
	void DespawnLaserDirect(size_t laserIndex);

	Display::Window* window;
	Game::Console* console;
	Game::Client* client;
	uint64 currentTime;
	uint64 timeDiff;

	std::vector<std::tuple<Render::ModelId, Physics::ColliderId, glm::mat4>> asteroids;

	std::vector<Game::Laser*> lasers;
	Render::ModelId laserModel;
	float laserSpeed;

	std::vector<Game::SpaceShip*> spaceShips;
	bool hasReceivedSpaceShip;
	uint32 controlledShipId;
	Game::SpaceShip* controlledShip;
	Render::ModelId spaceShipModel;
};