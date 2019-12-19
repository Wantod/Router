#pragma once

#include "Socket.hpp"
namespace net {

	struct addr
	{
		union {
			struct sockaddr         sa;
			struct sockaddr_in      s4;
			struct sockaddr_in6     s6;
			struct sockaddr_storage ss;
		} data;
	};
	
	inline bool is_ipv6(addr &address) {
		return (address.data.ss.ss_family == AF_INET6) ? true : false;
	}

	inline std::size_t get_size(addr &addr) {
		return (is_ipv6(addr) ? sizeof(sockaddr_in6) : sizeof(sockaddr_in6));
	}

	inline void set_data(addr &addr, const char *ipStr, const unsigned short int port, int family = AF_UNSPEC)
	{
		struct addrinfo hints, *res;
		int status;
		char port_buffer[6];

		sprintf(port_buffer, "%hu", port);

		std::memset(&hints, 0, sizeof(hints));
		hints.ai_family = family;
		hints.ai_socktype = SOCK_DGRAM;
		/* Setting AI_PASSIVE will give you a wildcard address if addr is NULL */
		hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV | AI_PASSIVE;

		if ((status = getaddrinfo(ipStr, port_buffer, &hints, &res)) != 0)
		{
			std::cerr << "[ERROR] getaddrinfo: " << gai_strerror(status) << std::endl;
			return;
		}

		/* Note, we're taking the first valid address, there may be more than one */
		std::memcpy(&addr.data.ss, res->ai_addr, res->ai_addrlen);
		::freeaddrinfo(res);
	}

	inline void set_port(addr &addr, uint16_t port) {
		if (addr.data.ss.ss_family == AF_INET)
			addr.data.s4.sin_port = htons(port);
		else
			addr.data.s6.sin6_port = htons(port);
	}

	inline uint16_t get_port(net::addr &addr) {
		return ntohs(is_ipv6(addr) ? addr.data.s6.sin6_port : addr.data.s4.sin_port);
	}

	inline void set_family(addr &addr, int family) {
		addr.data.ss.ss_family = family;
	}

	inline std::string to_string(addr &addr) {
		char hoststr[NI_MAXHOST];
		char portstr[NI_MAXSERV];
		
		int rc = getnameinfo((struct sockaddr *)&addr.data.sa, sizeof(struct sockaddr_storage),
								hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);

		return std::string(hoststr) + ":" + std::string(portstr);		
	}
}

