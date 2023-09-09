#pragma once

// --------------------
//		1차 시도
// --------------------

//struct SListEntry
//{
//	SListEntry* next;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

// --------------------
//		2차 시도
// --------------------

//struct SListEntry
//{
//	SListEntry* next;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

// ----------------------------------------
//		3차 시도(ABA Problem 해결 버전)
// ----------------------------------------

// Use-After-Free 문제는 존재 (Pop thread 1개면 안전, 여러개면 해결법을 찾아야 함)
// 16 바이트 정렬시 하위 4비트 0000으로 고정
DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;
};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	union
	{
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;
		
		struct
		{
			uint64 depth : 16;
			uint64 sequence : 48;
			uint64 reserved : 4;
			uint64 next : 60;
		} HeaderX64;
	};
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);