#pragma once

#include <chrono>

#include "CoreMinimal.h"
#include "Client/Client.hpp"
#include "GameFramework/Actor.h"
#include "GameCharacter/CharacterSystem.hpp"

#include "GameLoop.generated.h"

namespace YC::Client {
struct FPacBuf;
}

UCLASS()
class YC_UE_TEMPLATE01_API AGameLoop : public AActor {
	GENERATED_BODY()
	std::once_flag StartFlag;

	YC::Client::FPacBuf* PacBuf = nullptr;
	std::chrono::time_point<std::chrono::steady_clock> StartTime;
	YC::Client::FWorld World;
	int TickCount = 0;
public:
	AGameLoop();
	
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};