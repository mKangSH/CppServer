#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);
	PacketHeader header;

	br.Peek(&header);

	switch (header.id)
	{
	default:
		break;
	}
}

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

	BufferWriter bw(sendBuffer->Buffer(), 4096);
	PacketHeader* header = bw.Reserve<PacketHeader>();

	// id(uint64), ü��(uint32), ���ݷ�(uint16)
	bw << id << hp << attack;

	header->size = bw.WriteSize();
	header->id = 1; // 1: Test Msg

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}

