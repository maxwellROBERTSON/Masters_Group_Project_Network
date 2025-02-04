#include "GameClient.hpp"

static const uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = { 0 };

GameClient::GameClient(yojimbo::Address serverAddress) : serverAddress(serverAddress)
{
	// Initialise adapter
	adapter = YOJIMBO_NEW(yojimbo::GetDefaultAllocator(), GameAdapter);

	// Initialise  client
	double time = 1.0;
	client = YOJIMBO_NEW(yojimbo::GetDefaultAllocator(), yojimbo::Client,
		yojimbo::GetDefaultAllocator(),
		yojimbo::Address("0.0.0.0"),
		config,
		*adapter,
		time);
}

void GameClient::Connect()
{
	// Connect client
	uint64_t clientId;
	yojimbo_random_bytes((uint8_t*)&clientId, 8);
	client->InsecureConnect(DEFAULT_PRIVATE_KEY, clientId, serverAddress);
	//while (client->IsConnecting())
	//{
	//	std::cout << "Connecting to server...\n";
	//	// wait 5 seconds
	//	yojimbo_sleep(5.0);
	//	if (client->IsConnected())
	//	{
	//		break;
	//	}
	//}
	Run();
}

void GameClient::Run()
{
	// Run client
	float fixedDt = 1.0f / 120.0f;
	while (true)
	{
		client->SendPackets();

		client->ReceivePackets();

		if (client->IsDisconnected())
			break;

		time += fixedDt;

		client->AdvanceTime(time);

		if (client->ConnectionFailed())
			break;

		/*if(client->IsConnecting())
			std::cout << "Connecting to server...\n";*/

		yojimbo_sleep(fixedDt);
		GameMessage* message = (GameMessage*)adapter->factory->CreateMessage(GameMessageType::GAME_MESSAGE);
		message->sequence = 42;
		client->SendMessage(1, message);
		//adapter->factory->ReleaseMessage(message);
	}
	//Update(fixedDt);
	/*while (client->IsConnected())
	{
		double currentTime = yojimbo_time();
		if (client->GetTime() <= currentTime)
		{
			Update(fixedDt);
		}
		else
		{
			yojimbo_sleep(client->GetTime() - currentTime);
		}
	}*/
}

void GameClient::Update(float dt)
{
    // update client
    client->AdvanceTime(client->GetTime() + dt);
    client->ReceivePackets();

    if (client->IsConnected())
    {
        ProcessMessages();

        // ... do connected stuff ...

        // send a message when space is pressed
        /*if (KeyIsDown(Key::SPACE)) {
            TestMessage* message = (TestMessage*)m_client.CreateMessage((int)GameMessageType::TEST);
            message->m_data = 42;
            m_client.SendMessage((int)GameChannel::RELIABLE, message);
        }*/
        yojimbo::Message* message = adapter->factory->CreateMessage(1);
		client->SendMessage(0, message);
    }

    client->SendPackets();
}

void GameClient::ProcessMessages()
{
    for (int i = 0; i < config.numChannels; i++)
    {
        yojimbo::Message* message = client->ReceiveMessage(i);
        while (message != NULL)
        {
            ProcessMessage(message);
            client->ReleaseMessage(message);
            message = client->ReceiveMessage(i);
        }
    }
}

void GameClient::ProcessMessage(yojimbo::Message* message)
{
    std::cout << "Processing message from server with messageID " << message->GetId() << std::endl;
}

void GameClient::Disconnect()
{
	// Disconnect client
	client->Disconnect();
	YOJIMBO_DELETE(yojimbo::GetDefaultAllocator(), GameAdapter, adapter);
	YOJIMBO_DELETE(yojimbo::GetDefaultAllocator(), Client, client);
}