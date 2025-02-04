#include <iostream>

#include "../../third_party/yojimbo/include/yojimbo.h"
#include "GameClient.hpp"

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

	GameClient client(yojimbo::Address("127.0.0.1", 40000));

	client.Connect();

	while (true)
	{
		continue;
	}
	client.Disconnect();

    std::cout << "Hello World!\n";

	ShutdownYojimbo();

	return 0;
}
