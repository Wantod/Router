#pragma once

#include "../net/Packet.hpp"

namespace DNSParser
{
	// #pragma pack (1)
	// struct HeaderDNSPack {
	// 	uint16_t id, flags;
	// 	uint16_t QDcount, ANcount, NScount, ARcount;
	// };
	// #pragma pack ()

	struct HeaderDNS {
		u16 id;

		u8 r1, r2;
		u8 qr, opcode, aa, tc, rd, 	// r1
		   ra, rcode;				// r2

		u16 QDcount;
		u16 ANcount;
		u16 NScount;
		u16 ARcount;

		// Message
		char name[1024];
		u16 type;
		u16 classe;
	};

	HeaderDNS decode(ParserPacket &packet);
	std::vector<char> encode(HeaderDNS head);
	void display(HeaderDNS &mess);
};