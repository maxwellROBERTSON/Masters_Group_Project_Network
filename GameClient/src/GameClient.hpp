#include "../../third_party/yojimbo/include/yojimbo.h"
#include "GameAdapter.hpp"

class GameClient
{
public:
	GameClient(yojimbo::Address);
	void Connect();
	void Run();
	void Update(float dt);
	void ProcessMessages();
	void ProcessMessage(yojimbo::Message* message);
	void Disconnect();

private:
	yojimbo::Client* client;
	yojimbo::ClientServerConfig config;
	GameAdapter* adapter;
	yojimbo::Address serverAddress;
	double time = 1.0;
};
