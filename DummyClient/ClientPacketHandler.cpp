#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	// switch/case ��� �� ������ ã�� ��� hash table ��
	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	}
}

#pragma pack(1)
struct BuffData
{
	uint64 buffId;
	float remainTime;
};

// ��Ŷ ���� TEMP
struct PKT_S_TEST
{
	uint16 packetSize;
	uint16 packetId;

	uint64 id; // 8
	uint32 hp; // 4
	uint16 attack; // 2

	uint16 buffsOffset;
	uint16 buffsCount;
	// ���� ������
	// 1) ���ڿ� (ex.name) Encoding Issue
	// 2) �Ϲ� ����Ʈ �迭 (ex. ��� �̹���)
	// 3) �Ϲ� ����Ʈ
	
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

	uint64 id;
	uint32 hp;
	uint16 attack;
	br >> id >> hp >> attack;

	cout << "ID: " << id << " HP: " << hp << " ATT: " << attack << endl;

	vector<BuffData> buffs;
	uint16 buffCount;
	br >> buffCount;

	buffs.resize(buffCount);
	for (int32 i = 0; i < buffCount; i++)
	{
		br >> buffs[i].buffId >> buffs[i].remainTime;
	}

	cout << "BuffCount : " << buffCount << endl;
	for (int32 i = 0; i < buffCount; i++)
	{
		cout << "Buf Info : " << buffs[i].buffId << " " << buffs[i].remainTime << endl;
	}

	wstring name;
	uint16 nameLen;

	br >> nameLen;
	name.resize(nameLen);

	br.Read((void*)name.data(), nameLen * sizeof(WCHAR));

	wcout.imbue(std::locale("kor"));
	wcout << name << endl;
}
