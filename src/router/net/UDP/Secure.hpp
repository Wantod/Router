#pragma once

struct PacketUDP
{
    struct Header
    {
		uint16_t id;
		uint16_t size;
	};

    Header header;
    std::array<uint8_t, 1024> data;
};

enum class ConnectionStatus {
    Empty,
	Disconnect,
	Connecter,
	Lost,
	loged
};

struct UserConnectionStatus
{
    net::addr addr;
    ConnectionStatus status;
};

class PacketManager
{
public:
    std::array<>
}