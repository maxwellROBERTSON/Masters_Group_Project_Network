#include <iostream>

#include "../../third_party/yojimbo/include/yojimbo.h"
#include "GameServer.hpp"

int main()
{
    if (!InitializeYojimbo())
    {
        std::cerr << "Failed to initialize Yojimbo\n";
		return 1;
	}
	else
	{
		std::cout << "Yojimbo initialized\n";
	}

#ifdef DEBUG
	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
#else
	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
#endif

	GameServer server;
	server.Start();

	server.Stop();

    std::cout << "Hello World!\n";

	ShutdownYojimbo();

	return 0;
}
