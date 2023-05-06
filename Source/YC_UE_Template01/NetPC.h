#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetPC.generated.h"

UCLASS()
class YC_UE_TEMPLATE01_API ANetPC : public APlayerController {
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
public:
	void SendPacketToServer(uint8 ID, const TArray<uint8>& Packet) const;
	void SendPacketToClient(const uint8 ID, const TArray<uint8>& Packet) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	static void UI_MsgCall(FString Msg);
private:
	UFUNCTION(Server, Reliable)
	void S_ServerRecvPacket(uint8 ID, const TArray<uint8>& Packet) const ;

	UFUNCTION(Client, Reliable)
	void C_ClientRecvPacket(uint8 ID, const TArray<uint8>& Packet) const;
};