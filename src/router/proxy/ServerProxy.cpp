#include "../net/Socket.hpp"
#include "../net/Address.hpp"

#include "ServerProxy.hpp"
#include "HttpParser.hpp"

#include <sstream>
void HttpRequestParser(ParserRequest &parser, const std::string &buff)
{
	std::istringstream iss(buff);
	std::string line;
	std::getline(iss, line);
	std::cout << "------------------------\n";
	std::getline(iss, line, ' ');
	std::getline(iss, line, ':');

	// address
	parser.address[line.size()] = '\0';
	std::memcpy(parser.address, line.data(), line.size());

	std::getline(iss, line);

	// port
	line = line.substr(0, line.size() - 1);
	parser.port[line.size()] = '\0';
	std::memcpy(parser.port, line.data(), line.size());

	std::cout << "Parser : \"" << parser.address << "\" : \"" << parser.port << "\"" << std::endl;
}

SOCKET createServerProxy(ParserRequest &req)
{
	struct addrinfo ahints = {};
	struct addrinfo *paRes;

	ahints.ai_family = PF_UNSPEC;
	ahints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(req.address, req.port, &ahints, &paRes) != 0) {
		std::cerr << "Error getaddrinfo: " << net::GetError() << std::endl;
		return INVALID_SOCKET;
	}

	SOCKET sock = socket(paRes->ai_family, paRes->ai_socktype, paRes->ai_protocol);
	if (sock == SOCKET_ERROR) {
		std::cerr << "SOCKET: " << net::GetError() << std::endl;
		return INVALID_SOCKET;
	}
	if (::connect(sock, paRes->ai_addr, paRes->ai_addrlen) == SOCKET_ERROR) {
		std::cerr << "Connect: " << net::GetError() << std::endl;
		net::CloseSocket(sock);
		return INVALID_SOCKET;
	}

	freeaddrinfo(paRes);
	return sock;
}

void writeToSocket(SOCKET sockfd, const char *buff, int buff_length)
{
	int totalsend = 0;

	while (totalsend < buff_length) {
		int len = send(sockfd, buff + totalsend, buff_length - totalsend, 0);
		if (len < 0) {
			std::cerr << "Connect: " << net::GetError() << std::endl;
			return ;
		}
		totalsend += len;
	}
}

void writeToClient(SOCKET clientFD, SOCKET serverFD)
{
	size_t len = 0;
	constexpr int MAX_BUF_SIZE = 5000;
	char buff[MAX_BUF_SIZE];

	while ((len = recv(serverFD, buff, MAX_BUF_SIZE, 0)) > 0)
	{
		buff[len] = '\0';
		std::cout << buff << std::endl;
		writeToSocket(clientFD, buff, len);
	}

	if (len < 0)
	{
		std::cerr << "Error recv: " << net::GetError() << std::endl;
		return ;
	}
}

void newClient(SOCKET sockClient)
{
	constexpr int MAX_BUFFER_SIZE = 5000;
	char buff[MAX_BUFFER_SIZE + 1];
	std::string output;

	int recvd = MAX_BUFFER_SIZE;
	while (recvd == MAX_BUFFER_SIZE)
	{
		recvd = ::recv(sockClient, buff, MAX_BUFFER_SIZE, 0);
		std::cout << recvd << " - data\n";
		if (recvd < 0) {
			return ;
		} else if (recvd == 0) {
			break;
		}

		buff[recvd] = '\0';
		int size = output.size();
		output.resize(output.size() + recvd);
		std::memcpy(output.data() + size, buff, recvd);
	}

	std::cout << output << std::endl;
	ParserRequest request;
	HttpRequestParser(request, output);

	SOCKET sockServ = createServerProxy(request);
	if (sockServ != SOCKET_ERROR)
	{
		writeToSocket(sockServ, output.data(), output.size());
		writeToClient(sockClient, sockServ);
		net::CloseSocket(sockServ);
	}
}

bool runProxy(int port)
{
	SOCKET sockFD = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFD == SOCKET_ERROR) {
		std::cerr << "Erreur socket: " << net::GetError() << std::endl;
		return false;
	}

	struct sockaddr_in serverAddr = {};
	struct sockaddr_in clientAddr = {};

	serverAddr.sin_family = AF_INET;			// IPv4 Family 
	serverAddr.sin_addr.s_addr = INADDR_ANY;	// localhost / 127.0.0.1 
	serverAddr.sin_port = htons(port);

	if (::bind(sockFD, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		std::cerr << "ERROR bind: " <<  net::GetError() << std::endl;
		return false;
	}

	// maximum 100 requets
	if (::listen(sockFD, 100) == SOCKET_ERROR) {
		std::cerr << "ERROR listen: " <<  net::GetError() << std::endl;
		return false;
	}

	while (1) {
		socklen_t lenClient;
		SOCKET sockClient = accept(sockFD, reinterpret_cast<sockaddr *>(&clientAddr), &lenClient);
		if (sockClient == SOCKET_ERROR)
		{
			std::cerr << "ERROR: Request limited crossed" << std::endl;
			continue;
		}

		std::cout << "Proxy: ip: " << Address::toString(clientAddr) << std::endl;
		newClient(sockClient);
		net::CloseSocket(sockClient);		
	}

	net::CloseSocket(sockFD);
	return true;
}