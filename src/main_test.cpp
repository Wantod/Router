#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch.hpp>
#include <iostream>
#include <chrono>
#include <thread>

#include "router/net/Packet.hpp"
#include "router/net/PacketStack.hpp"
#include "router/net/Address.hpp"
#include "router/net/addr.hpp"
#include "router/net/UDP/UDPSocket.hpp"

#include "router/math/Hash.hpp"
#include "router/math/sha1.hpp"
#include "router/math/map_interval.hpp"
#include "router/math/BinaryCheck.hpp"

#include "router/net/testSocket.hpp"

// TEST_CASE("Binary Check") {
// 	std::vector<std::string> _data = {
// 		"Saltt",
// 		"Pourquoi",
// 		"Sast",
// 		"Pourqua"
// 	};

// 	BinaryCheck binary;
// 	binary.compile(_data);
// 	binary.showTree();
// }


TEST_CASE("map_interval") {

	char value = 'X';
	interval_map obj;
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < 2048; i++)
		{
			obj.insert(i, i, 'A');
			if (i < 10 && i > 15)
			{
				i++;
				obj.insert(i, i, 'B');
			}
			// obj.insert(6, 8, 'C');
			// obj.insert(2, 2, 'B');
			// obj.insert(3, 9, 'C');
			// obj.insert(4, 30, 'E');
			// obj.insert(1, 10, 'X');
		}
		auto stop = std::chrono::high_resolution_clock::now();
		obj.print_map();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "interval: " << (int)duration.count() << "ms\n";
		// CHECK(duration.count() < 100);
	}

	{
		CHECK(obj.lowDico(10) == obj.low(10));
		CHECK(obj.lowDico(2) == obj.low(2));
		CHECK(obj.lowDico(1000) == obj.low(1000));
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		uint16_t val = obj.get(1000);
		auto stop = std::chrono::high_resolution_clock::now();
		CHECK(val == 'X');
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "interval: " << (int)duration.count() << "ms\n";
		CHECK(duration.count() < 100);
	}

	// char c = obj[9];
	// CHECK(c == 'K');
}

/*
// http://www.microhowto.info/howto/listen_for_and_accept_tcp_connections_in_c.html#idp58640
TEST_CASE("test_socket") {
	net::Start();

	int sockServer = create_listen_socket(NULL, 8080);
	REQUIRE(sockServer != SOCKET_ERROR);

	std::thread t1([&] {
		struct sockaddr_storage sa;
		socklen_t sa_len=sizeof(sa);
		std::cout << "wating ..." << std::endl;
		int session_fd = accept(sockServer, (struct sockaddr*)&sa, &sa_len);
		std::cout << "new client id:" << session_fd << std::endl;

		CHECK(session_fd != SOCKET_ERROR);
		char buff[255];
		int size = recv(session_fd, buff, 255, 0);
		CHECK(size > 0);
		std::cout.write(buff, size);

		char buffer[INET6_ADDRSTRLEN];
		int err = getnameinfo((struct sockaddr*)&sa,sa_len,buffer,sizeof(buffer),0,0,NI_NUMERICHOST);
		if (err != 0) {
			snprintf(buffer, sizeof(buffer), "invalid address");
		}
		REQUIRE(std::string(buffer) == "::1.");
		net::CloseSocket(session_fd);
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int sockClient = create_connect_socket("127.0.0.1", 8080);
	if (sockClient == SOCKET_ERROR)
	{
		CHECK(sockClient != SOCKET_ERROR);
		net::CloseSocket(sockClient);
		net::Release();
		return ;
	}

	send(sockClient, "GET / HTTP/1.0\n\n", 16, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	net::CloseSocket(sockServer);
	net::CloseSocket(sockClient);
	t1.join();
	net::Release();
}

// TEST_CASE("HTTP REQUEST") {
// 	int sockClient = create_connect_socket("google.com", 80);
// 	REQUIRE(sockClient != SOCKET_ERROR);
// 	send(sockClient, "GET / HTTP/1.0\n\n", 16, 0);
// 	char buffer[512];
// 	int bytes;
// 	do {
// 		bytes = recv(sockClient, buffer, 512, 0 );
 
// 		if ( bytes > 0 )
// 			std::cout.write(buffer, bytes);
// 	} while ( bytes > 0 );
// }

TEST_CASE("HASH") {
	const char buff[255] = { "Hello world" };
	auto start = std::chrono::high_resolution_clock::now();
	uint32_t crc = rc_crc32(0, buff, 11);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	CHECK(crc == 0x8BD69E52);
	CHECK(duration.count() < 100);

	sha1 hash;
	hash.update("Hello world");
	CHECK(hash.final() == "7b502c3a1f48c8609ae212cdfb639dee39673f5e");
}

TEST_CASE("[addr_new_v6]" ) {
	net::Start();

	net::addr clientAddr;
	net::set_data(clientAddr, "127.0.0.1", 8080);
	REQUIRE(net::to_string(clientAddr) == "127.0.0.1:8080");
	REQUIRE(net::is_ipv6(clientAddr) == false);

	net::set_data(clientAddr, "::1", 8080);
	REQUIRE(net::to_string(clientAddr) == "::1:8080");
	REQUIRE(net::is_ipv6(clientAddr) == true);


	net::Release();
}

TEST_CASE("[packet]" ) {
	struct DataTest {
		u32 a;
		u8 b;
		u16 c;
	};

	Packet packet(400);

	// write
	DataTest data = { 3, 2, 0xABCD };
	packet.write(data.a);
	packet.write(data.b);
	packet.write(data.c);
	packet.write(data.c);
	packet.write(data.c);
	packet.write(data.c);
	std::cout << "Packet: " << packet.toString() << "\n";

	// read
	DataTest dataRead = {};
	packet >> dataRead.a >> dataRead.b >> dataRead.c;
	REQUIRE(dataRead.a == 3); // get info packet
	REQUIRE(dataRead.b == 2); // get info packet
	REQUIRE(dataRead.c == 0xABCD); // get info packet
}


TEST_CASE("[packet_recv]" ) {
	u8 buff[254];
	for (u8 i = 0; i < 254; i++) {
		buff[i] = i;
	}

	Packet packet(400);
	std::memcpy(packet.data(254), buff, 254 * sizeof(u8));
	for (u8 i = 0; i < 254; i++)
	{
		u8 c;
		packet >> c;
		if (c != i) REQUIRE(c == i); // error in read buffer to packet
	}
}

TEST_CASE("[packet_move]" ) {
	Packet packet(1040);
	packet << 1 << 4 << 5 << 6 << 7 << 8;
	Packet packet2(std::move(packet));
	REQUIRE("01-00-00-00-04-00-00-00-05-00-00-00-06-00-00-00-07-00-00-00-08-00-00-00" == packet2.toString());
}

TEST_CASE("[packet_stack]") {
	PacketStack packet;
	unsigned int port = packet.run("127.0.0.1");
	(void) port;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

#include "router/dns/ServerDNS.hpp"

TEST_CASE("dns_serveur") {
	net::Start();

	ServerDNS dns;
	if (dns.init(53) == false) return ;

	std::thread t1([&] { dns.run(); });


	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	dns.stop();
	t1.join();

	net::Release();
}

// #include "router/proxy/ServerProxy.hpp"
// void test_proxy()
// {
// 	runProxy(5050);
// }

TEST_CASE("addr", "[!benchmark]") {
    net::Addr addr(0x01010199, 10);
    CHECK(addr.toString() == "1.1.1.153:10");
}

TEST_CASE("[udp_server]")
{
	net::Start();

    UDPSocket server;
    server.init(true);
    server.bind(0u);

    std::thread t1([&] {
        Packet packet;
        net::addr cliAddr = {};
		cliAddr.data.ss.ss_family = AF_INET;

        packet.resize(250);
        // std::cout << "Attendre recv" << std::endl;
        // // msg 1
        CHECK(server.recv(packet, cliAddr) != -1);
        CHECK(packet.toString() == "00-FF-11-FF-02-00-00-00-03-00-00-00-05-00-00-00");
        // msg 2
        CHECK(server.recv(packet, cliAddr) != 1);
        CHECK(packet.toString() == "00-00-00-00-01-00-00-00-02-00-00-00-0A-00-00-00");
	});

    net::addr addrServ = {};
	net::set_data(addrServ, "127.0.0.1", net::get_port(server.getSocket()));
	std::cout << "SERVER ADDR: " << net::to_string(addrServ) << " v6: " << (net::is_ipv6(addrServ) ? "true" : "false") << std::endl;
    // client
    UDPSocket client;
    client.init(false);
    Packet p;

	{
		p << 0xFF11FF00 << 2 << 3 << 5;
		client.send(p, addrServ);
	}
    p.clear();
	{
		p << 0u << 1u << 2u << 10u;
		client.send(p, addrServ);
	}
    // force shutdown after 5s
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    client.close();
    server.close();
    t1.join();

	net::Release();
}
*/