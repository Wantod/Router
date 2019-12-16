#pragma once

#include "Socket.hpp"
#include "Packet.hpp"
#include "Address.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

class UDPSocket
{
	public:
		UDPSocket();
		~UDPSocket();

		int recv(void *data, std::size_t size, net::Addr &addr);
		bool send(const void *data, std::size_t size, net::Addr &addr);

		int recv(Packet &p, net::Addr &addr);
		bool send(const Packet &ee, net::Addr &addr);

		bool wait();
		bool bind(unsigned short port = 0);
		bool init(bool blocking = false);

		void close();

		net::Addr getSocket();
	private:
		SOCKET sock;
		SOCKADDR_IN server;
};
