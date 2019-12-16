#pragma once

#include "../Type.hpp"
#include "Socket.hpp"

namespace net
{
	inline u8 host_to_network(u8) { };

	inline u16 host_to_network(u16 i) { return htons(i); }
	inline u32 host_to_network(u32 i) { return htonl(i); }
	inline i32 host_to_network(i32 i) { return htonl(i); }
	inline u16 network_to_host(u16 i) { return ntohs(i); }
	inline u32 network_to_host(u32 i) { return ntohl(i); }
	inline i32 network_to_host(i32 i) { return ntohl(i); }


	enum class StateTCP {
		CONNECT,
		WRITABLE,
		EOF,
		DESTOYING
	};
}