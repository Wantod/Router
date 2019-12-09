#pragma once

# include <iostream>
# include <atomic>

# include <thread>
# include <mutex>

#include <queue>
#include "Packet.hpp"

#include <chrono>

class PacketStack
{
public:
	PacketStack() : _end(false), _access(false)
	{}

	~PacketStack()
	{
		_end = true;
		if (_thread.get() != nullptr) {
			_thread->join();
		}
	}

	void stop() { _end = true; }
	std::queue<Packet> getPackets() {
		std::scoped_lock lock(_mutex);
		return std::move(_packets);
	}

	void setPacket(Packet packet) {
		std::scoped_lock lock(_mutex);
		_packets.push(std::move(packet));
	}

	// if port = 0 return port assigned auto
	unsigned int run(std::string addr, unsigned int port = 0) {
		_thread = std::make_unique<std::thread>([&] { recvThread(); });

		return port;
	}

private:
	std::mutex _mutex;
	std::queue<Packet> _packets;
	std::atomic<bool> _end;
	std::atomic<bool> _access;
	std::unique_ptr<std::thread> _thread;

private:
	void recvThread() {
		Packet packet(1050);

		while (_end == false)
		{
			std::cout << "Test" << std::endl;
			std::this_thread::sleep_for(std::chrono::microseconds(200000));

			// recv packet
			// add packet
			// setPacket(std::move(packet));
		}
	}
};