#include "UDPSocket.hpp"

UDPSocket::UDPSocket() {

}

UDPSocket::~UDPSocket() {
	this->close();
}

void UDPSocket::close() {
	if (sock != 0)
	{
		net::CloseSocket(sock);
		sock = 0;
	}
}

bool UDPSocket::init(bool blocking) {
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // ipv4, UDP

	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Erreur initialisation socket : " << net::GetError() << std::endl;
		return false;
	}

	if (!blocking && !net::SetNonBlocking(sock))
	{
		std::cerr << "Erreur settings non bloquant : " << net::GetError() << std::endl;
		return false;
	}

	return true;
}

net::Addr UDPSocket::getSocket() {
	return net::Addr(server);
}

bool UDPSocket::bind(unsigned short port) {
	server.sin_addr.s_addr = INADDR_ANY;  // indique que toutes les sources seront accept�es
										  // UTILE: si le port est 0 il est assign� automatiquement
	server.sin_port = htons(port); // toujours penser � traduire le port en r�seau
	server.sin_family = AF_INET; // notre socket est UDP
	socklen_t size = sizeof(server);

	getsockname(sock, (struct sockaddr *) &server, &size);
	
	if (::bind(sock, reinterpret_cast<SOCKADDR *>(&server), sizeof(server)) == SOCKET_ERROR)
	{
		std::cerr << "Erreur bind : " << net::GetError() << std::endl;
		return false;
	}

	getsockname(sock, (struct sockaddr *) &server, &size);

	return true;
}

bool UDPSocket::wait() {
	fd_set readset;
	timeval timeout = { 0 };
	int selectRedy;

	FD_ZERO(&readset);

	FD_SET(sock, &readset);
	selectRedy = select(sock + 1, &readset, nullptr, nullptr, &timeout);
	if (selectRedy == -1)
	{
		std::cerr << "Erreur select : " << net::GetError() << std::endl;
		return false;
	}
	else if (selectRedy > 0)
	{
		if (FD_ISSET(sock, &readset))
		{
			return true;
		}
	}

	return false;
}

int UDPSocket::recv(void *data, std::size_t size, net::Addr &addr)
{
	int sinsize = sizeof (addr.sin);

	int n = recvfrom(sock, static_cast<char*>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&addr.sin), &sinsize);
	if (n < 0)
		return -1;
	return n;
}

bool UDPSocket::send(const void *data, std::size_t size, net::Addr &addr)
{
	if (sendto(sock, static_cast<const char*>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&addr.sin), sizeof (addr.sin)) < 0)
	{
		std::cerr << "Erreur send()" << net::GetError() << std::endl;
		return false;
	}

	return true;
}

int UDPSocket::recv(Packet &packet, net::Addr & addr)
{
	int sinsize = sizeof(addr.sin);

	int n = recvfrom(sock, packet.data(), static_cast<int>(packet.size()), 0, reinterpret_cast<sockaddr*>(&addr.sin), &sinsize);
	if (n < 0)
		return -1;
	packet.resize(n);
	return n;
}

bool UDPSocket::send(const Packet &packet, net::Addr & addr)
{
	if (::sendto(sock, packet.data(), static_cast<int>(packet.size()), 0, reinterpret_cast<sockaddr*>(&addr.sin), sizeof(addr.sin)) < 0)
	{
		std::cerr << "Erreur send(Packet): " << net::GetError() << std::endl;
		return false;
	}

	return true;
}
