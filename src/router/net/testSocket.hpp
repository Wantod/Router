#pragma once

#include "Socket.hpp"

inline int create_listen_socket(const char *host, int port)
{
	struct addrinfo hints, *res;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

    char port_string[16];
    snprintf(port_string, 16, "%d", port);
	int status;
    if ((status = getaddrinfo(host, port_string, &hints, &res)) != 0) {
        std::cerr << "[ERROR] getaddrinfo: " << gai_strerror(status) << std::endl;
		return SOCKET_ERROR;
    }

	int fd = SOCKET_ERROR;
	struct addrinfo *listenAddr;

	for (struct addrinfo *a = res; a && fd == SOCKET_ERROR; a = a->ai_next) {
        if (a->ai_family == AF_INET6) {
            fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
            listenAddr = a;
			std::cout << "IP v6 found" << std::endl;
        }
    }

    for (struct addrinfo *a = res; a && fd == SOCKET_ERROR; a = a->ai_next) {
        if (a->ai_family == AF_INET) {
            fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
            listenAddr = a;
			std::cout << "IP v4 found" << std::endl;
        }
    }

	if (fd == SOCKET_ERROR) {
        freeaddrinfo(res);
        return SOCKET_ERROR;
    }

	// forcer l'allocation du ports
	int enabled = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *) &enabled, sizeof(enabled)) == -1) {
		std::cout << "Erreur set socket opt" << std::endl;
	}


	if (::bind(fd, listenAddr->ai_addr, (socklen_t) listenAddr->ai_addrlen) == SOCKET_ERROR || listen(fd, 512)) {
        net::CloseSocket(fd);
        freeaddrinfo(res);
        return SOCKET_ERROR;
    }

    int disabled = 1;
    setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (const char *) &disabled, sizeof(disabled));
	setsockopt(fd, SOL_SOCKET, SO_RCVLOWAT, (const char *)&disabled,sizeof(disabled));

	freeaddrinfo(res);
	return fd;
}


inline int create_connect_socket(const char *host, int port) {
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char port_string[16];
    snprintf(port_string, 16, "%d", port);

    if (getaddrinfo(host, port_string, &hints, &result) != 0) {
        return SOCKET_ERROR;
    }

    int fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd == SOCKET_ERROR) {
        freeaddrinfo(result);
        return SOCKET_ERROR;
    }

    connect(fd, result->ai_addr, (socklen_t) result->ai_addrlen);
    freeaddrinfo(result);
    return fd;
}