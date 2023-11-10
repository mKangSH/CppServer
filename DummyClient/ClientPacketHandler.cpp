#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	// switch/case 대신 더 빠르게 찾을 방법 hash table 등
	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	}
}

// 패킷 설계 TEMP
struct S_TEST
{
	uint64 id;
	uint32 hp;
	uint16 attack;
};

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	// TEMP
	PacketHeader header;
	br >> header;

	uint64 id;
	uint32 hp;
	uint16 attack;
	br >> id >> hp >> attack;

	cout << "ID: " << id << "HP: " << hp << "ATT: " << attack << endl;
}
