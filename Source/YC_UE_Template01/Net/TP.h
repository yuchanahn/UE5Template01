#pragma once

#include "CoreMinimal.h"
#include "Packets.hpp"
#include "UObject/Object.h"
#include "TP.generated.h"

UCLASS()
class YC_UE_TEMPLATE01_API UTP : public UObject {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FTestPacket {
	GENERATED_BODY()
	
	UPROPERTY()
	int32 TestInt;
	UPROPERTY()
	float TestFloat;
	UPROPERTY()
	FString ChatMassage;
	
	// Macro
	static constexpr uint8 __packet_id = 0;
	static void Bind(std::function<void(FTestPacket, int)> Fn) {
		__packet_events[__packet_id] = [Fn](const TArray<uint8>& Data, const int Client_ID) {
			Fn(YC::PRW::Deserialize<FTestPacket>(Data), Client_ID);
		};
	}
	static void ServerBind(std::function<void(FTestPacket, int)> Fn) {
		__server_packet_events[__packet_id] = [Fn](const TArray<uint8>& Data, const int Client_ID) { 
			Fn(YC::PRW::Deserialize<FTestPacket>(Data), Client_ID); 
		};
	}
};