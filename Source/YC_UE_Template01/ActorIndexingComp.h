﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorIndexingComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YC_UE_TEMPLATE01_API UActorIndexingComp : public UActorComponent {
	GENERATED_BODY()

public:
	UActorIndexingComp();

	UPROPERTY(ReplicatedUsing=OnRep_Variable)
	int32 OwnID;

	UFUNCTION()
	void OnRep_Variable();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
