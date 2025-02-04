#include <iostream>
#include <regex>

#include "../../third_party/yojimbo/include/yojimbo.h"
#include "GameClient.hpp"

bool isValidIPAddress(const std::string& ip) {
	// Define the regex pattern for IPv4 and IPv6
	const std::string ipPattern = R"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)";

	// Create a regex object
	std::regex ipRegex(ipPattern);

	// Check if the IP matches the regex pattern
	return std::regex_match(ip, ipRegex);
}

int main()
{
	std::cout << "Enter a IP address: ";
	std::string ip;
	std::cin >> ip;
	if (!isValidIPAddress(ip))
	{
		std::cerr << "Invalid IP address\n";
		return 1;
	}

	std::cout << "Enter a port: ";
	std::string port;
	std::cin >> port;
	for (int i = 0; i < port.length(); i++)
	{
		if (!isdigit(port[i]))
		{
			std::cerr << "Non-integer port\n";
			return 1;
		}
	}

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

	GameClient client(yojimbo::Address(ip.c_str(), std::stoi(port)));

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
