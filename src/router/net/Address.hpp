#pragma once

#include "Socket.hpp"
#include "../Type.hpp"

#include <string>
#include <sstream>

struct Address
{
	static unsigned char Address::GetByte(u32 ip, int n) {
		return ((unsigned char*)&ip)[3 - n];
	}

	static u32 getIp(SOCKADDR_IN &sin) { return ntohl(sin.sin_addr.s_addr); }
	static u16 getPort(SOCKADDR_IN &sin) { return ntohs(sin.sin_port); }

	static std::string toString(SOCKADDR_IN &sin)
	{
		u32 ip = getIp(sin);
		u16 port = getPort(sin);
		
		std::stringstream ss;
		ss << (int)GetByte(ip, 0) << '.' << (int)GetByte(ip, 1) << '.' << (int)GetByte(ip, 2) << '.' << (int)GetByte(ip, 3) << ':' << (int)port;
		return ss.str();
	}
};