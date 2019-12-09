#include <iostream>
#include "test/test.hpp"
#include "router/net/Packet.hpp"
#include "router/net/PacketStack.hpp"

bool test_packet() {
	std::cout << "test_packet - start" << std::endl;
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
	EQUAL(dataRead.a, 3, "get info packet");
	EQUAL(dataRead.b, 2, "get info packet");
	EQUAL(dataRead.c, 0xABCD, "get info packet");

	std::cout << "test_packet - end" << std::endl;
	return true;
}

bool test_packet_recv() {
	std::cout << "test_packet_recv - start" << std::endl;
	// recv
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
		if (c != i) EQUAL(c, i, "error in read buffer to packet");
	}

	std::cout << "test_packet_recv - end" << std::endl;
	return 0;
}

void test_packet_move()
{
	std::cout << "test_packet_move - start" << std::endl;
	Packet packet(1040);
	packet << 1 << 4 << 5 << 6 << 7 << 8;
	std::cout << packet.toString() << "\n";

	Packet packet2(std::move(packet));
	std::cout << packet2.toString() << "\n";
	std::cout << "test_packet_move - end" << std::endl;
}

#include <chrono>
#include <thread>
void test_packet_stack()
{
	std::cout << "test_packet_stack - start" << std::endl;
	PacketStack packet;
	unsigned int port = packet.run("127.0.0.1");
	(void) port;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "test_packet_stack - end" << std::endl;
}

#include "router/dns/ServerDNS.hpp"

void test_dns_serveur()
{
	ServerDNS dns;
	if (dns.init(53) == false) return ;

	std::thread t1([&] {
		dns.run();
	});
	std::this_thread::sleep_for(std::chrono::seconds(30));
	dns.stop();
	t1.join();	
}

#include "router/proxy/ServerProxy.hpp"
void test_proxy()
{
	net::Start();
	runProxy(5050);
	net::Release();
}

int main() {
	std::cout << "Test" << std::endl;

	test_packet();
	test_packet_recv();
	test_packet_move();
	test_packet_stack();
	// test_dns_serveur();
	test_proxy();

	// int a = 2;
	// CHECK(1 == 1); // success
	// CHECK(a != 2); // error
	// EQUAL(a, 2);

	displaySumaryError();
	return 0;
}