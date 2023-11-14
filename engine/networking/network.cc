#include "config.h"
#include "network.h"

namespace Game
{
	bool isNumber(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}

	void splitStringSpace(const std::string& arg, std::string& IP, enet_uint16& port) {
		//Check if arg is empty
		if (arg == "") {
			port = 1234;
			return;
		}
		IP = arg.substr(0, arg.find(" ")); //Split on space

		std::string secondHalf = arg.substr(arg.find(" ") + 1, -1);
		if (isNumber(secondHalf))
			port = stoi(secondHalf);
		else
		{
			port = 1234;
			printf("\n[ERROR] Invalid input port. Used 1234 instead.\n");
		}
	}

#pragma region data

	PeerData::PeerData()
	{
		sender = nullptr;
	}

	PeerData::PeerData(ENetPeer* _sender, enet_uint8* _data, size_t dataSize)
	{
		sender = _sender;
		data = std::make_shared<std::vector<enet_uint8>>();
		data->reserve(dataSize);
		for (size_t i = 0; i < dataSize; i++)
			data->push_back(_data[i]);
	}

	PeerData::PeerData(const PeerData& other) //Copy
	{
		sender = other.sender;
		data = other.data;
	}

	PeerData& PeerData::operator=(const PeerData& other)
	{
		sender = other.sender;
		data = other.data;
		return *this;
	}

	PeerData::~PeerData() {}

#pragma endregion data

#pragma region host

	Host::Host(HostType _type) :
		type(_type),
		host(nullptr)
	{}

	Host::~Host()
	{
		enet_host_destroy(host);
	}

	void Host::Update()
	{
		ENetEvent event;
		while (enet_host_service(host, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				OnConnect(event.peer);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				dataStack.push_back(PeerData(event.peer, event.packet->data, event.packet->dataLength));
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				OnDisconnect(event.peer);
				break;
			}
		}
	}

	bool Host::PopDataStack(PeerData& outData)
	{
		if (dataStack.size() == 0)
			return false;

		outData = dataStack.back();
		dataStack.pop_back();
		return true;
	}

	void Host::SendData(void* data, size_t byteSize, ENetPeer* peer, ENetPacketFlag packetFlag)
	{
		if (peer == nullptr)
		{
			printf("\n[ERROR] tried to send data to nullptr peer.\n");
			return;
		}

		ENetPacket* packet = enet_packet_create(data, byteSize, packetFlag);
		enet_peer_send(peer, 0, packet);
		enet_host_flush(host);
	}

#pragma endregion host

#pragma region server

	Server::Server() :
		Host(HostType::Server),
		onClientConnect(nullptr),
		onClientDisconnect(nullptr)
	{}

	Server::~Server()
	{}

	bool Server::Init(const char* serverIP, enet_uint16 port, std::function<void(ENetPeer*)> _onClientConnect, std::function<void(ENetPeer*)> _onClientDisconnect)
	{
		onClientConnect = _onClientConnect;
		onClientDisconnect = _onClientDisconnect;


		ENetAddress address;
		enet_address_set_host(&address, serverIP);
		address.port = port;

		host = enet_host_create(&address, 32, 2, 0, 0);

		if (host == nullptr)
		{
			printf("\n[ERROR] failed to create ENet server host.\n");
			return false;
		}

		printf("\n[INFO] server created.\n");
		return true;
	}

	void Server::Broadcast(void* data, size_t byteSize, ENetPacketFlag packetFlag, ENetPeer* exlude)
	{
		ENetPacket* packet = enet_packet_create(data, byteSize, packetFlag);

		if (exlude == nullptr)
		{
			enet_host_broadcast(host, 0, packet);
		}
		else
		{
			for (auto& peer : connectedPeers)
			{
				if (peer != exlude)
					enet_peer_send(peer, 0, packet);
			}
		}

		enet_host_flush(host);
	}

	void Server::OnConnect(ENetPeer* peer)
	{
		if (connectedPeers.count(peer) == 0)
		{
			connectedPeers.insert(peer);
			onClientConnect(peer);
		}
	}

	void Server::OnDisconnect(ENetPeer* peer)
	{
		onClientDisconnect(peer);
		connectedPeers.erase(peer);
	}
	#pragma endregion server

#pragma region client

	Client::Client() :
		Host(HostType::Client),
		onServerConnect(nullptr),
		onServerDisconnect(nullptr),
		server(nullptr)
	{}

	Client::~Client()
	{}

	bool Client::Init(std::function<void(ENetPeer*)> _onServerConnect, std::function<void(ENetPeer*)> _onServerDisconnect)
	{
		onServerConnect = _onServerConnect;
		onServerDisconnect = _onServerDisconnect;

		host = enet_host_create(nullptr, 1, 2, 0, 0);

		if (host == nullptr)
		{
			printf("\n[ERROR] failed to create ENet client host.\n");
			return false;
		}

		printf("\n[INFO] client created.\n");
		return true;
	}

	bool Client::TryConnecting(const char* serverIP, enet_uint16 port)
	{
		ENetAddress address;
		ENetEvent event;

		enet_address_set_host(&address, serverIP);
		address.port = port;

		server = enet_host_connect(host, &address, 2, 0);

		if (server == nullptr)
		{
			printf("\n[ERROR] failed to initiate ENet connection.\n");
			return false;
		}
		if (enet_host_service(host, &event, 5000) > 0 &&
			event.type == ENET_EVENT_TYPE_CONNECT)
		{
			puts("Connection to serverIP succeeded.");
		}
		else
		{
			enet_peer_reset(server);
			puts("Connection to serverIP failed.");
		}

		return true;
	}

	void Client::OnConnect(ENetPeer* peer)
	{
		onServerConnect(server);
	}

	void Client::OnDisconnect(ENetPeer* peer)
	{
		onServerDisconnect(server);
		server = nullptr;
	}
}
#pragma endregion client