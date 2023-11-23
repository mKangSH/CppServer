#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "Protocol.pb.h"

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

// [PKT_S_TEST] [BuffsListItem BuffsListItem BuffsListItem]
void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	Protocol::S_TEST pkt;

	ASSERT_CRASH(pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)));

	cout << pkt.id() << " " << pkt.hp() << " " << pkt.attack() << endl;
	cout << "Buff Size: " << pkt.buffs_size() << endl;

	for (auto& buf : pkt.buffs())
	{
		cout << "Buff Info: " << buf.buffid() << " " << buf.remaintime() << endl;
		cout << "Victims: " << buf.victims_size() << endl;
		for (auto& vic : buf.victims())
		{
			cout << vic << " ";
		}

		cout << endl;
	}
}
