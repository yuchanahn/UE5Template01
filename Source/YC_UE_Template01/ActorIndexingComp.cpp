#include "ActorIndexingComp.h"

#include <format>

#include "Client/Client.hpp"
#include "Net/UnrealNetwork.h"



UActorIndexingComp::UActorIndexingComp() {
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UActorIndexingComp::OnRep_Variable() {
	YC::Client::PrintUELog_S(std::format("{}:{}", "OnRep_Variable", OwnID).c_str());
	YC::Client::ANetEntity_List[OwnID] = this;
}

void UActorIndexingComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActorIndexingComp, OwnID);
}


void UActorIndexingComp::BeginPlay() {
	Super::BeginPlay();
	if(GetOwner()->HasAuthority()) {
		YC::Client::PrintUELog("UActorIndexingComp::BeginPlay");
		YC::Client::ANetEntity_List[OwnID] = this;
	}
}

void UActorIndexingComp::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	YC::Client::ANetEntity_List.erase(OwnID);
}

void UActorIndexingComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

