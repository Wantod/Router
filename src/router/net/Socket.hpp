#ifndef SOCKET_HPP
#define SOCKET_HPP

#ifdef _WIN32
	typedef unsigned long nfds_t;
	#define poll WSAPoll
	#if !defined(_MSC_VER)
		#include <WS2tcpip.h>
    #elif _MSC_VER >= 1800
		#include <WS2tcpip.h>
	#else
		typedef int socklen_t;
	#endif

	#define inet_ntop(FAMILY, PTR_STRUCT_SOCKADDR, BUFFER, BUFFER_LENGTH) strncpy(BUFFER, inet_ntoa(*static_cast<struct in_addr*>((PTR_STRUCT_SOCKADDR))), BUFFER_LENGTH)
	#define inet_pton(FAMILY, IP, PTR_STRUCT_SOCKADDR) (*(PTR_STRUCT_SOCKADDR)) = inet_addr((IP))

	#ifdef _MSC_VER
		#if _WIN32_WINNT == _WIN32_WINNT_WINBLUE
			//!< Win8.1 & higher
			#pragma comment(lib, "Ws2_32.lib")
		#else
			#pragma comment(lib, "wsock32.lib")
			#pragma comment (lib, "Ws2_32.lib")
		#endif
	#endif
#else
	#include <sys/socket.h>
	#include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
	#include <arpa/inet.h> // hton*, ntoh*, inet_addr
	#include <unistd.h>  // close
	#include <poll.h>
	#include <netdb.h> // int adrress
#endif

#include <iostream>
#include <string>
#include "Error.hpp"

namespace net
{
	bool Start();
	void Release();
	bool SetNonBlocking(SOCKET socket);
	void CloseSocket(SOCKET socket);
	void Shutdown(SOCKET socket);
	std::string GetAddress(const sockaddr_in& addr);
}

#endif // SOCKET_HPP
