#include "Hash.hpp"

uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len)
{
	static uint32_t table[256];
	static bool have_table = false;
 
    // Calcule CRC table
	if (have_table == false) {
		for (int i = 0; i < 256; i++) {
			uint32_t rem = i;  /* remainder from polynomial division */
			for (int j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = true;
	}
 
	crc = ~crc;
	const char *q = buf + len;
	for (const char *p = buf; p < q; p++) {
		uint8_t octet = static_cast<uint8_t>(*p);
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}