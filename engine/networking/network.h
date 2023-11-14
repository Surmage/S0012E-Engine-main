#pragma once
#include "enet/enet.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <functional>
#include "string"

namespace Game
{
	void splitStringSpace(const std::string& arg, std::string& IP, enet_uint16& port);

	struct PeerData
	{
		ENetPeer* sender;
		std::shared_ptr<std::vector<enet_uint8>> data;

		PeerData();
		PeerData(ENetPeer* _sender, enet_uint8* _data, size_t dataSize);
		PeerData(const PeerData& other);
		PeerData& operator=(const PeerData& other);
		~PeerData();
	};

	enum class HostType
	{
		Client,
		Server
	};

	class Host
	{
	protected:
		ENetHost* host;
		std::vector<PeerData> dataStack;
		virtual void OnConnect(ENetPeer* peer) = 0;
		virtual void OnDisconnect(ENetPeer* peer) = 0;

	public:
		HostType type;

		Host(HostType _type);
		~Host();

		void Update();
		bool PopDataStack(PeerData& outData);
		void SendData(void* data, size_t byteSize, ENetPeer* peer, ENetPacketFlag packetFlag);
		
	};

	class Server : public Host
	{
	private:
		std::function<void(ENetPeer*)> onClientConnect;
		std::function<void(ENetPeer*)> onClientDisconnect;
		virtual void OnConnect(ENetPeer* peer) override;
		virtual void OnDisconnect(ENetPeer* peer) override;

	public:
		std::unordered_set<ENetPeer*> connectedPeers;

		Server();
		~Server();

		bool Init(const char* serverIP, enet_uint16 port, std::function<void(ENetPeer*)> _onClientConnect, std::function<void(ENetPeer*)> _onClientDisconnect);
		void Broadcast(void* data, size_t byteSize, ENetPacketFlag packetFlag, ENetPeer* exlude = nullptr);
		
	};

	class Client : public Host
	{
	private:
		std::function<void(ENetPeer*)> onServerConnect;
		std::function<void(ENetPeer*)> onServerDisconnect;
		virtual void OnConnect(ENetPeer* peer) override;
		virtual void OnDisconnect(ENetPeer* peer) override;

	public:
		ENetPeer* server;

		Client();
		~Client();

		bool Init(std::function<void(ENetPeer*)> _onServerConnect, std::function<void(ENetPeer*)> _onServerDisconnect);
		bool TryConnecting(const char* serverIP, enet_uint16 port);
		
	};
}