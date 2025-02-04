#include <iostream>

#include "GameServer.hpp"
#include "../../third_party/yojimbo/source/yojimbo_allocator.cpp"

static const uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = { 0 };

GameServer::GameServer()
{
	// Initialise adapter
	adapter = YOJIMBO_NEW(yojimbo::GetDefaultAllocator(), GameAdapter);

	// Initialise server
	double time = 1.0;
	maxClients = 2;
	server = YOJIMBO_NEW(yojimbo::GetDefaultAllocator(), yojimbo::Server,
		yojimbo::GetDefaultAllocator(),
		DEFAULT_PRIVATE_KEY,
		yojimbo::Address("129.11.146.144", 40000),
		config,
		*adapter,
		time);

	adapter->SetServer(server);
}

void GameServer::Start()
{
	// Start server
	server->Start(maxClients);
	Run();
}

void GameServer::Run()
{
	float fixedDt = 1.0f / 120.0f;
	while (server->IsRunning())
	{
		double currentTime = yojimbo_time();
		if (server->GetTime() <= currentTime)
		{
			Update(fixedDt);
		}
		else
		{
			yojimbo_sleep(server->GetTime() - currentTime);
		}
	}
}

void GameServer::Update(float fixedDt)
{
	// stop if server is not running
	if (!server->IsRunning()) 
	{
		Stop();
		return;
	}

	// update server and process messages
	server->AdvanceTime(server->GetTime() + fixedDt);
	server->ReceivePackets();
	ProcessMessages();

	// ... process client inputs ...
	// ... update game ...
	// ... send game state to clients ...

	server->SendPackets();
}

void GameServer::ProcessMessages()
{
	for (int i = 0; i < maxClients; i++)
	{
		if (server->IsClientConnected(i))
		{
			for (int j = 0; j < config.numChannels; j++)
			{
				GameMessage* message = (GameMessage*)server->ReceiveMessage(i, j);
				while (message != NULL)
				{
					ProcessMessage(i, message);
					server->ReleaseMessage(i, message);
					message = (GameMessage*)server->ReceiveMessage(i, j);
				}
			}
		}
	}
}

void GameServer::ProcessMessage(int clientIndex, GameMessage* message)
{
	//std::cout << "Processing message from client " << clientIndex << " with messageID " << message->GetId() << std::endl;
	std::cout << "MESSAGE FROM CLIENT " << clientIndex << " " << message->sequence << " MESSAGEID = " << message->GetId() << std::endl;
}

void GameServer::Stop()
{
	// Stop server
	server->Stop();
	YOJIMBO_DELETE(yojimbo::GetDefaultAllocator(), GameAdapter, adapter);
	YOJIMBO_DELETE(yojimbo::GetDefaultAllocator(), Server, server);
}
