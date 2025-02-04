#include <iostream>
#include <cstdlib>

#include "../../third_party/yojimbo/include/yojimbo.h"
#include "GameServer.hpp"

#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>

yojimbo::Address GetLocalIPAddress(uint16_t port) {
    WSADATA wsaData;
    char hostname[256];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        yojimbo::Address address;
        return address;
    }

    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        std::cerr << "Error getting hostname" << std::endl;
        WSACleanup();
        yojimbo::Address address;
        return address;
    }

    struct addrinfo hints = { 0 }, * info, * p;
    hints.ai_family = AF_INET;

    if (getaddrinfo(hostname, NULL, &hints, &info) != 0) {
        std::cerr << "getaddrinfo failed!" << std::endl;
        WSACleanup();
        yojimbo::Address address;
        return address;
    }

    std::string add_str = "";

    struct sockaddr_in* addr = (struct sockaddr_in*)info->ai_addr;
	add_str = inet_ntoa(addr->sin_addr);

	yojimbo::Address address(add_str.c_str(), port);
    freeaddrinfo(info);
    WSACleanup();
	return address;
}

#elif defined(OS_LINUX_MAC)
//#include <sys/types.h>
//#include <ifaddrs.h>
//#include <arpa/inet.h>
//#include <netinet/in.h>
//#include <cstring>
//
//void GetLocalIPAddress() {
//    struct ifaddrs* ifAddrStruct = NULL;
//    struct ifaddrs* ifa = NULL;
//    void* tmpAddrPtr = NULL;
//
//    if (getifaddrs(&ifAddrStruct) == -1) {
//        std::cerr << "Error getting network interfaces" << std::endl;
//        return;
//    }
//
//    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
//        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
//            tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
//            char addressBuffer[INET_ADDRSTRLEN];
//            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
//            std::cout << "Interface: " << ifa->ifa_name << " - IP Address: " << addressBuffer << std::endl;
//        }
//    }
//
//    if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
//}

#endif

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <max_clients>\n";
		return 1;
	}

	for (int i = 0; i < strlen(argv[1]); i++)
	{
		if (!isdigit(argv[1][i]))
		{
			std::cerr << "Non-integer port number\n";
			return 1;
		}
	}
	for (int i = 0; i < strlen(argv[2]); i++)
	{
		if (!isdigit(argv[2][i]))
		{
			std::cerr << "Non-integer max clients\n";
			return 1;
		}
	}

    int port = std::strtol(argv[1], nullptr, 10);
	int maxClients = std::strtol(argv[2], nullptr, 10);

	if (port < 0 || port > 65535)
    {
		std::cerr << "Invalid port number\n";
		std::cerr << "Port number must be between 0 and 65535\n";
		return 1;
    }
	if (maxClients < 1 || maxClients > 64)
	{
		std::cerr << "Invalid max clients\n";
		std::cerr << "Max clients must be between 1 and 64\n";
		return 1;
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

	// start server at address of machine and max clients from command line
	GameServer server(GetLocalIPAddress(port), 2);
	server.Start();

	server.Stop();

    std::cout << "Hello World!\n";

	ShutdownYojimbo();

	return 0;
}
