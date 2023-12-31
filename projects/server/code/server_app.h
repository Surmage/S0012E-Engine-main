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
#include <unordered_map>

class ServerApp : public Core::App
{
public:
	ServerApp();
	~ServerApp();

	bool Open();
	void Run();
	void Exit();

	void OnClientConnect(ENetPeer* client);
	void OnClientDisconnect(ENetPeer* client);

private:
	void InitSpawnPoints();

	// update functions
	void RenderUI();
	void UpdateNetwork();
	void UpdateSpaceShips(float deltaTime);
	void UpdateLasers();

	// unpack messages from client
	void PackPlayer(Game::SpaceShip* spaceShip, Protocol::Player& p_player);
	void PackLaser(Game::Laser* laser, Protocol::Laser& p_laser);
	void HandleMsgInput(ENetPeer* sender, const Protocol::PacketWrapper* packet);
	void HandleMsgText(ENetPeer* sender, const Protocol::PacketWrapper* packet);

	// methods that send data to the clients
	void SpawnSpaceShip(ENetPeer* client);
	void UpdateSpaceShipData(ENetPeer* client);
	void DespawnSpaceShip(ENetPeer* client);
	void RespawnSpaceShip(ENetPeer* client);
	void SendGameState(ENetPeer* client);
	void SendClientConnect(ENetPeer* client);
	void SpawnLaser(const glm::vec3& origin, const glm::quat& direction, uint32 spaceShipId, uint64 currentTimeMillis);
	void DespawnLaser(size_t index);

	Display::Window* window;
	Game::Console* console;
	Game::Server* server;
	uint64 currentTime;

	std::vector<std::tuple<Render::ModelId, Physics::ColliderId, glm::mat4>> asteroids;

	std::unordered_map<ENetPeer*, Game::SpaceShip*> spaceShips;
	Render::ModelId spaceShipModel;
	uint32 nextSpaceShipId;
	std::vector<glm::vec3> spawnPoints;
	float spaceShipCollisionRadiusSquared;

	std::vector<Game::Laser*> lasers;
	Render::ModelId laserModel;
	uint32 nextLaserId;
	uint64 laserMaxTime;
	float laserSpeed;
	float laserCooldown;
};