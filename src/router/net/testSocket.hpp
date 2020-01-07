#pragma once

#include "Socket.hpp"

inline int create_listen_socket(const char *host, int port)
{
	struct addrinfo hints, *res;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

    char port_string[16];
    snprintf(port_string, 16, "%d", port);
	int status;
    if ((status = getaddrinfo(host, port_string, &hints, &res)) != 0) {
        std::cerr << "[ERROR] getaddrinfo: " << gai_strerror(status) << std::endl;
		return;
    }

	int fd = SOCKET_ERROR;
	struct addrinfo *listenAddr;

	for (struct addrinfo *a = res; a && fd == SOCKET_ERROR; a = a->ai_next) {
        if (a->ai_family == AF_INET6) {
            fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
            listenAddr = a;
        }
    }

    for (struct addrinfo *a = res; a && fd == SOCKET_ERROR; a = a->ai_next) {
        if (a->ai_family == AF_INET) {
            fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
            listenAddr = a;
        }
    }

	if (fd == SOCKET_ERROR) {
        freeaddrinfo(res);
        return ;
    }

	int enabled = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (SETSOCKOPT_PTR_TYPE) &enabled, sizeof(enabled));
}