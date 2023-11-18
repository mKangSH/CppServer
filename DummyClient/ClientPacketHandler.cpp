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

#pragma pack(1)
// [PKT_S_TEST] [BuffData BuffData BuffData]
struct PKT_S_TEST
{
	struct BuffsListItem
	{
		uint64 buffId;
		float remainTime;
	};

	uint16 packetSize;	// 2
	uint16 packetId;	// 2
	uint64 id;			// 8
	uint32 hp;			// 4
	uint16 attack;		// 2
	uint16 buffsOffset; // 2
	uint16 buffsCount;	// 2

	bool Validate()
	{
		uint32 size = 0;

		size += sizeof(PKT_S_TEST);
		size += buffsCount * sizeof(BuffsListItem);

		if (size != packetSize)
		{
			return false;
		}

		if (buffsOffset + buffsCount * sizeof(buffsCount) > packetSize)
		{
			return false;
		}

		return true;
	}
	// vector<BuffData> buffs;
	// wstring name;
};
#pragma pack()

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	if (len < sizeof(PKT_S_TEST))
	{
		return;
	}

	PKT_S_TEST pkt;
	br >> pkt;

	if (pkt.Validate() == false)
	{
		return;
	}

	vector<PKT_S_TEST::BuffsListItem> buffs;

	buffs.resize(pkt.buffsCount);
	for (int32 i = 0; i < pkt.buffsCount; i++)
	{
		br >> buffs[i];
	}

	cout << "BuffCount : " << pkt.buffsCount << endl;
	for (int32 i = 0; i < pkt.buffsCount; i++)
	{
		cout << "Buf Info : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;
	}
}
