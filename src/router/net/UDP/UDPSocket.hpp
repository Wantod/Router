#pragma once

#include "../Socket.hpp"
#include "../Packet.hpp"
#include "../addr.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

class UDPSocket
{
	public:
		UDPSocket();
		~UDPSocket();

		int recv(void *data, std::size_t size, net::addr &addr);
		bool send(const void *data, std::size_t size, net::addr &addr);

		int recv(Packet &p, net::addr &addr);
		bool send(const Packet &ee, net::addr &addr);

		bool wait();
		bool bind(unsigned short port = 0);
		bool init(bool blocking = false, bool ipv6 = false);

		void close();

		net::addr getSocket();

	private:
		SOCKET sock;
		sockaddr_in6  server;
};
