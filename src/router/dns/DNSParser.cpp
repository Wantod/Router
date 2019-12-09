#include "DNSParser.hpp"
#include <iostream>
#include <sstream>

// https://www.frameip.com/dns/ DNS PROTOCOL

DNSParser::HeaderDNS DNSParser::decode(ParserPacket &packet)
{
	DNSParser::HeaderDNS header;
	std::cout << packet.toString() << std::endl;
	packet	>> header.id
			>> header.r1 >> header.r2
			>> header.QDcount >> header.ANcount
			>> header.NScount >> header.ARcount; // 12 octets
	
	header.qr 		= (header.r1 & 0b10000000) >> 7;
	header.opcode 	= (header.r1 & 0b01111000) >> 3;
	header.aa	 	= (header.r1 & 0b00000100) >> 2;
	header.tc	 	= (header.r1 & 0b00000010) >> 1;
	header.rd	 	= (header.r1 & 0b00000001) >> 0;

	header.ra 		= (header.r2 & 0b10000000) >> 7;
	header.rcode	= (header.r2 & 0b00001111) >> 0;

	int len = packet.read<u8>();
	int lenMax = packet.size() - packet.sizeRead() - 1;
	int j = 0;
	while (len != 0 && j < lenMax)
	{
		if (j != 0)
		{
			header.name[j] = '.';
			j++;
		}

		for (int i = 0; i < len; i++)
		{
			header.name[j] = packet.read<char>();
			j++;
		}

		len = packet.read<u8>();
	}

	header.name[j] = '\0';
	header.type = packet.read<u16>();
	header.classe = packet.read<u16>();

	display(header);
	return header;
}

void DNSParser::display(HeaderDNS &mess)
{
	std::ostringstream text;
    text << std::endl << "QUERY { " << std::endl;
    text << "\tID: " << std::showbase << std::hex << mess.id << std::endl << std::noshowbase;
    text << "\tfields: [ QR: " << mess.qr << " opCode: " << mess.opcode << " ]" << std::endl;
    text << "\tQDcount: " << mess.QDcount << std::endl;
    text << "\tANcount: " << mess.ANcount << std::endl;
    text << "\tNScount: " << mess.NScount << std::endl;
    text << "\tARcount: " << mess.ARcount << std::endl;
	// Query
    text << "\tName: " << std::string(mess.name) << std::endl;
    text << "\tType: " << mess.type << std::endl;
    text << "\tClass: " << mess.classe << std::endl;
    text << " }" << std::endl;

    std::cout << text.str();
}