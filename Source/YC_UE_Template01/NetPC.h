#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetPC.generated.h"

UCLASS()
class YC_UE_TEMPLATE01_API ANetPC : public APlayerController {
	GENERATED_BODY()
public:
	void SendPacketToServer(uint8 ID, const TArray<uint8>& Packet);
	void SendPacketToClient(const TArray<uint8>& Packet, int ClientId);
	
private:
	UFUNCTION(Server, Reliable)
	void S_ServerRecvPacket(uint8 ID, const TArray<uint8>& Packet);
	
	UFUNCTION(NetMulticast, Reliable)
	void NMC_ServerRecvPacket(uint8 ID, const TArray<uint8>& Packet);
};