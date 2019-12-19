#pragma once

#include "Socket.hpp"
#include "../Type.hpp"

#include <string>
#include <sstream>

namespace net
{
	class Addr
	{
	public:
		Addr(u32 ip = 0, u16 port = 0) {
			sin = {};
			set(ip, port);
		}
		Addr(SOCKADDR_IN &_sin) : sin(_sin) {}

		void set(u32 ip = 0, u16 port = 0) {
			sin.sin_addr.s_addr = htonl(ip);
			sin.sin_port = htons(port);
		}

		u32 getIp() const	{ return ntohl(sin.sin_addr.s_addr); }
		u16 getPort() const	{ return ntohs(sin.sin_port); }

		net::Addr get() {
			return net::Addr(this->getIp(), this->getPort());
		}

		std::string toString() const {
			return net::GetAddress(sin) + ":" + std::to_string((int)getPort());
		}

		bool set(const char *address, unsigned short port) {
			struct hostent *hostinfo;
			hostinfo = gethostbyname(address);
			if (hostinfo == NULL)
			{
				std::cerr << "Unknown host " << address << "." << std::endl;
				return false;
			}

			sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
			sin.sin_port = htons(port);
			sin.sin_family = AF_INET;
			return true;
		}

		SOCKADDR_IN sin;
	};
}