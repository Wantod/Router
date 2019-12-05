
#include <iostream>

#include "router/net/Socket.hpp"
// #define SIO_RCVALL _WSAIOW(IOC_VENDOR,1) //this removes the need of mstcpip.h

int main() {
	net::Start();

	int addrFamily = AF_INET; // AF_INET6

	SOCKET sniffer = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sniffer == INVALID_SOCKET) {
		std::cerr << "INVALID_SOCKET\n";
		return 1;
	}

	char hostname[100];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		std::cerr << "ERROR: " <<  net::GetError() << std::endl;
		return 1;
	}
	std::cout << "Hostname: " << hostname << std::endl;


	struct hostent *local = gethostbyname(hostname);
	if (local == NULL) {
		std::cerr << "ERROR: " <<  net::GetError() << std::endl;
		return 1;
	}

	struct in_addr addr;
	int max_interface = -1;
	for (int i = 0; local->h_addr_list[i] != 0; ++i)
	{
		max_interface = i;
		memcpy(&addr, local->h_addr_list[i], sizeof(struct in_addr));
		printf("Interface Number : %d Address : %s\n",i,inet_ntoa(addr));
	}


	int num_interface = 0;
	// std::cin >> num_interface;
	if (num_interface < 0 || num_interface > max_interface) {
		std::cout << "Interface Empty" << std::endl;
		return 1;
	}

	struct sockaddr_in source, dest;
	memset(&dest, 0, sizeof(dest));
	memcpy(&dest.sin_addr.s_addr, local->h_addr_list[num_interface], sizeof(dest.sin_addr.s_addr));
	dest.sin_family = AF_INET;
	dest.sin_port = 0;

	// int j = 1;
	// if (WSAIoctl(sniffer, SIO_RCVALL, &j, sizeof(j), 0, 0, (LPDWORD) &num_interface , 0 , 0) == SOCKET_ERROR)
	// {
	// 	printf("WSAIoctl() failed.\n");
	// 	return 1;
	// }

	// struct in_addr addr;
	// struct hostent *local;

	// bind(sniffer, (struct sockaddr *)&dest, sizeof(dest));


	// memcpy(&dest.sin_addr.s_addr, local - &h_addr_list[in],
	// 	sizeof(dest.sin_addr.s_addr));

	// dest.sin_family = AF_INET;
	// dest.sin_port = 0;
	// dest.sin_zero = 0;

	net::CloseSocket(sniffer);
	net::Release();

	return 0;
}