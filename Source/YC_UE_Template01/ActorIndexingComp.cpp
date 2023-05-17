#include "ActorIndexingComp.h"

#include <format>

#include "Client/Client.hpp"
#include "Net/UnrealNetwork.h"



UActorIndexingComp::UActorIndexingComp() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UActorIndexingComp::OnRep_Variable() {
}

void UActorIndexingComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActorIndexingComp, OwnID);
}


void UActorIndexingComp::BeginPlay() {
	Super::BeginPlay();
	SetIsReplicated(true);
}

void UActorIndexingComp::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void UActorIndexingComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

