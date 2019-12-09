#pragma once

#include "../net/Packet.hpp"

namespace DNSParser
{
	struct HeaderDNS {
		u16 id;
		u8 r1, r2;

		u8 qr, opcode, aa, tc, rd, ra, rcode;	

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
	void display(HeaderDNS &mess);
};