#pragma once

#include "../Type.hpp"
#include <vector>

template <typename T>
T swap_endian(T u) {
	union
	{
		T u;
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for (std::size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T) - k - 1];

	return dest.u;
}

class ParserPacket
{
public:
	ParserPacket(const char *data, std::size_t size)
	{
		_data = data;
		_size = size;
		_read = 0;
		_write = 0;
	}

	template <typename T>
	T read()
	{
		T nb = *reinterpret_cast<const T *>(&_data[_read]);
		_read += sizeof(T);
		return swap_endian(nb);
	}

	template <typename T>
	void write(T nb)
	{
		T nb2 = swap_endian(nb);
		std::memcpy(&nb2, _data + _write, sizeof(T));
		_write += sizeof(T);
		return 
	}

	template<typename T>
	ParserPacket & operator >> (T &data) {
		data = read<T>();
		return *this;
	}

	template<typename T>
	ParserPacket & operator << (T data) {
		write(data);
		return *this;
	}

	std::string toString() const {
		return toStringHexa(_data, _size);
	}

	std::string toStringHexa(const char *data, std::size_t size) const {
		if (size == 0) return "";
		char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		std::string str_out;
		str_out.resize((size * 3) - 1, '-');
		
		int i = 0;
		for (size_t j = 0; j < size; j++)
		{
			char character = data[j];
			str_out[i + 0] = hex_chars[ ( character & 0xF0 ) >> 4 ];
			str_out[i + 1] = hex_chars[ ( character & 0x0F ) >> 0 ];
			i += 3;
		}

		return str_out;
	}

	const char *data() const { return _data; }
	const std::size_t size() const { return _size; }
	const std::size_t sizeRead() const { return _read; }
	

private:
	const char *_data;
	std::size_t _size;

	std::size_t _read;
	std::size_t _write;
};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  WARNING - is only little endien
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
class Packet {
public:
	Packet(std::size_t size = 0) : _read(0), _write(0)
	{
		std::cout << "Packet()\n";
		_data.reserve(size);
	}

	Packet(Packet &&p) :
		_data(std::move(p._data)),
		_read(std::exchange(p._read, 0)),
		_write(std::exchange(p._write, 0))
	{
		std::cout << "Packet(Packet&&)\n";
	}

	~Packet() {
		std::cout << "~Packet()\n";
	}

	std::size_t size() const { return _data.size(); }
	void resize(std::size_t size = 0) { _data.resize(size); }
	void clear() {
		_data.clear();
		_read = 0;
		_write = 0;
	}
	char* data() const {
		return (char *)(&_data[0]);
	}

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

	template<typename T>
	Packet & operator << (T data) {
		append(&data, sizeof(T));
		return *this;
	}

	std::string toString() const {
		if (_data.size() == 0) return "";
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