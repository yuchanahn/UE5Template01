#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLoop.generated.h"

UCLASS()
class YC_UE_TEMPLATE01_API AGameLoop : public AActor {
	GENERATED_BODY()
	bool StartFlag = false;
public:
	AGameLoop();
	virtual ~AGameLoop() override;
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;
};