#pragma once

#include "../net/Socket.hpp"
#include <atomic>

class ServerDNS
{
public:
	ServerDNS() : _socketFD(0), _end(false)
	{
		net::Start();
	}
	~ServerDNS()
	{
		if (_socketFD != 0)
		{
			net::CloseSocket(_socketFD);
		}
		net::Release();
	}

	bool init(int port);
	void run();

	void stop() {
		_end = true;
		if (_socketFD != 0) {
			net::CloseSocket(_socketFD);
		}
	}

private:
	static constexpr int BUFFER_SIZE = 1024;
	SOCKET _socketFD;
	std::atomic<bool> _end;
};
