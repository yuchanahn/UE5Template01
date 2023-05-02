#pragma once
#include "CoreMinimal.h"
#include "Packets.hpp"
#include "PRW.hpp"
#include "UObject/Object.h"
#include "PacketTypes.generated.h"



#define YC_PACKET(Type) inline static uint8 __packet_id = YC_Internal::GetPacketID();			        \
	static void Bind(std::function<void(Type)> Fn) {													\
		__packet_events[__packet_id] = [Fn](const TArray<uint8>& Data) {								\
			Fn(YC::PRW::Deserialize<Type>(Data));														\
		};																								\
	}																									\
	static void ServerBind(std::function<void(Type, int)> Fn) {											\
		__server_packet_events[__packet_id] = [Fn](const TArray<uint8>& Data, const int Client_ID) {    \
			Fn(YC::PRW::Deserialize<Type>(Data), Client_ID);                                            \
		};                                                                                              \
	}           																						\

namespace YC_Internal{
static uint8 __packet_id_counter = 0;
static uint8 GetPacketID() {
	return __packet_id_counter++;
}
}

/* =============== Packet =============== */
// 패킷 정의.
// 패킷은 반드시 YC_PACKET 매크로를 사용하여 정의해야 합니다.
// =========================================

struct FPac_Test;
struct FPac_ClientConnected;

/* =============== Chanel =============== */
// 패킷 채널링.
// 각 패킷의 채널을 정의 합니다.
// 같은 채널의 패킷들은 순서가 보장됩니다.
// =========================================
using Chanel_Player = std::variant<FPac_Test, FPac_ClientConnected>;

USTRUCT(BlueprintType)
struct FPac_Test {
	GENERATED_BODY()
	
	UPROPERTY()
	int32 TestInt;
	UPROPERTY()
	float TestFloat;
	UPROPERTY()
	FString ChatMassage;
	
	YC_PACKET(FPac_Test);
};

USTRUCT(BlueprintType)
struct FPac_ClientConnected {
	GENERATED_BODY()
	
	UPROPERTY()
	FString ChatMassage;
	
	YC_PACKET(FPac_ClientConnected);
};

USTRUCT()
struct FPac_ClientTimeSync {
	GENERATED_BODY()
	
	UPROPERTY()
	int64 ServerTimestamp;
	
	UPROPERTY()
	int64 Tick;
	
	YC_PACKET(FPac_ClientTimeSync);
};

USTRUCT()
struct FPac_SpawnAndPossess {
	GENERATED_BODY()
	
	UPROPERTY()
	int8 SelectedCharacterID;
	
	YC_PACKET(FPac_SpawnAndPossess);
};