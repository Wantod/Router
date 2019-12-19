#include "ServerDNS.hpp"
#include "DNSParser.hpp"
#include "../net/Address.hpp"
#include "../net/Packet.hpp"

bool ServerDNS::init(int port) {
	_socketFD = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(static_cast<u_short>(port));

	if (::bind(_socketFD, reinterpret_cast<struct sockaddr *>(&addr), sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		std::cerr << "ERROR bind: " <<  net::GetError() << std::endl;
		return false;
	}

	std::cout << "Listen in port: " << port << ", sock: " << _socketFD << std::endl;
	return true;
}

void ServerDNS::run() {
	_end = false;
	std::cout << "Serveur DNS run ..." << std::endl;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in addrClient = {};
	socklen_t lenSock = sizeof(struct sockaddr_in);

	while (_end == false)
	{
		int len = recvfrom(_socketFD, buffer, BUFFER_SIZE, 0, reinterpret_cast<struct sockaddr *>(&addrClient), &lenSock);	
		if (len == -1) continue;

		std::cout << "Packet reçu: " << len << " " << net::GetAddress(addrClient) << std::endl;
		ParserPacket parser(buffer, len);
		DNSParser::decode(parser);
	}

	std::cout << "STOP STOP\n";
}