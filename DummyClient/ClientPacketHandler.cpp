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
// [PKT_S_TEST] [BuffsListItem BuffsListItem BuffsListItem]
struct PKT_S_TEST
{
	struct BuffsListItem
	{
		uint64 buffId;
		float remainTime;

		uint16 victimsOffset;
		uint16 victimsCount;

		bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size)
		{
			if (victimsOffset + victimsCount * sizeof(uint64) > packetSize)
			{
				return false;
			}

			size += victimsCount * sizeof(uint64);
			return true;
		}
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
		if (packetSize < size)
		{
			return false;
		}

		if (buffsOffset + buffsCount * sizeof(buffsCount) > packetSize)
		{
			return false;
		}

		size += buffsCount * sizeof(BuffsListItem);

		BuffsList buffList = GetBuffsList();
		for (int32 i = 0; i < buffList.Count(); i++)
		{
			if (buffList[i].Validate((BYTE*)this, packetSize, OUT size) == false)
			{
				return false;
			}
		}

		if (size != packetSize)
		{
			return false;
		}

		return true;
	}

	// Flat Buffer
	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;
	using BuffsVictimsList = PacketList<uint64>;

	BuffsList GetBuffsList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsOffset;

		return BuffsList(reinterpret_cast<PKT_S_TEST::BuffsListItem*>(data), buffsCount);
	}

	BuffsVictimsList GetBufferVictimList(BuffsListItem* buffsItem)
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsItem->victimsOffset;

		return BuffsVictimsList(reinterpret_cast<uint64*>(data), buffsItem->victimsCount);
	}

	// vector<BuffData> buffs;
	// wstring name;
};
#pragma pack()

// [PKT_S_TEST] [BuffsListItem BuffsListItem BuffsListItem]
void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PKT_S_TEST* pkt = reinterpret_cast<PKT_S_TEST*>(buffer);

	if (pkt->Validate() == false)
	{
		return;
	}

	PKT_S_TEST::BuffsList buffs = pkt->GetBuffsList();

	cout << "BuffCount : " << buffs.Count() << endl;
	for (int32 i = 0; i < buffs.Count(); i++)
	{
		cout << "Buf Info : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;

		PKT_S_TEST::BuffsVictimsList victims = pkt->GetBufferVictimList(&buffs[i]);

		for (int32 j = 0; j < victims.Count(); j++)
		{
			cout << "Victim : " << victims[j] << endl;
		}
	}

	//for (auto it = buffs.begin(); it != buffs.end(); ++it)
	//{
	//	cout << "Buf Info : " << it->buffId << " " << it->remainTime << endl;
	//}

	//for (auto& buff : buffs)
	//{
	//	cout << "Buf Info : " << buff.buffId << " " << buff.remainTime << endl;
	//}
}
