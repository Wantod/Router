#include <iostream>
#include "test/test.hpp"
#include "router/net/Packet.hpp"

bool test_packet() {
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

	return true;
}

bool test_packet_recv() {
	Packet packet(400);
	// recv
	u8 buff[254];
	for (u8 i = 0; i < 254; i++) {
		buff[i] = i;
	}

	std::memcpy(packet.data(254), buff, 254 * sizeof(u8));

	for (u8 i = 0; i < 254; i++)
	{
		u8 c;
		packet >> c;
		if (c != i) EQUAL(c, i, "error in read buffer to packet");
	}

	return 0;
}

int main() {
	std::cout << "Test" << std::endl;

	test_packet();
	test_packet_recv();

	// int a = 2;
	// CHECK(1 == 1); // success
	// CHECK(a != 2); // error
	// EQUAL(a, 2);

	displaySumaryError();
	return 0;
}