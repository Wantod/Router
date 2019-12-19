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
#include "router/net/UDPSocket.hpp"

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
    server.init(true, true);
    server.bind(0u);

    std::thread t1([&] {
        Packet packet;
        net::addr cliAddr = {};

        packet.resize(250);
        std::cout << "Attendre recv" << std::endl;
        // msg 1
        server.recv(packet, cliAddr);
        CHECK(packet.toString() == "00-FF-11-FF-02-00-00-00-03-00-00-00-05-00-00-00");
		std::cout << "CLIENT -> Serveur >>>>>>>>>>" << net::to_string(cliAddr) << std::endl;
        // msg 2
        server.recv(packet, cliAddr);
        CHECK(packet.toString() == "00-00-00-00-01-00-00-00-02-00-00-00-0A-00-00-00");
		std::cout << "CLIENT -> Serveur >>>>>>>>>>" << net::to_string(cliAddr) << std::endl;
	});

    net::addr addrServ;
	net::set_data(addrServ, "::1", net::get_port(server.getSocket()));
	std::cout << "SERVER ADDR: " << net::to_string(addrServ) << " v6: " << (net::is_ipv6(addrServ) ? "true" : "false") << std::endl;
    // client
    UDPSocket client;
    client.init(false, true);
	// std::cout << "CLIENT ADDR: " << net::to_string(client.getSocket()) << " v6: " << net::is_ipv6(addrServ) << std::endl;
    Packet p;
    p << 0xFF11FF00 << 2 << 3 << 5;
    client.send(p, addrServ);
    p.clear();
    p << 0u << 1u << 2u << 10u;
    client.send(p, addrServ);

    // force shutdown after 5s
    std::this_thread::sleep_for(std::chrono::seconds(5));
    client.close();
    server.close();
    t1.join();

	net::Release();
}
