#pragma once

#include "../Type.hpp"
#include <vector>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  WARNING - is only little endien
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
class Packet {
public:
	Packet(std::size_t size) : _read(0), _write(0)
	{
		_data.reserve(size);
	}

	~Packet() {}

	void* data(std::size_t size) {
		if (_data.size() < size) {
			std::size_t sizeP = _data.size();
			_data.resize(size - sizeP);
		}
		return reinterpret_cast<void *>(&_data[0]);
	}

	void append(const void *data, std::size_t size) {
		std::size_t start = _data.size();
		_data.resize(start + size);
		std::memcpy(&_data[start], data, size);
	}

	template<typename T>
	void write(const T &v) {
		append(&v, sizeof(T));
	}

	template<typename T>
	Packet & operator >>(T &data) {
		if (_data.size() >= _read + sizeof(T)) {
			data = *reinterpret_cast<const T *>(&_data[_read]);
			_read += sizeof(T);
		}
		return *this;
	}

	template<typename T>
	Packet & operator << (T &data) {
		append(&data, sizeof(T));
		return *this;
	}

	std::string toString() {
		char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		std::string str_out;
		str_out.resize((_data.size() * 3) - 1, '-');
		
		int i = 0;
		for (u8 character : _data)
		{
			str_out[i + 0] = hex_chars[ ( character & 0xF0 ) >> 4 ];
			str_out[i + 1] = hex_chars[ ( character & 0x0F ) >> 0 ];
			i += 3;
		}

		return str_out;
	}

private:
	std::vector<u8> _data;
	u16 _read;
	u16 _write;
};