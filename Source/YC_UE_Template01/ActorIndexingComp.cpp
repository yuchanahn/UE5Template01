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
	YC::Client::AChrPtr_List[OwnID] = Cast<ACharacter>(GetOwner());
}

void UActorIndexingComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActorIndexingComp, OwnID);
}


void UActorIndexingComp::BeginPlay() {
	Super::BeginPlay();
	if(GetOwner()->HasAuthority()) {
		static int32 ID = 0;
		YC::Client::PrintUELog("UActorIndexingComp::BeginPlay");
		OwnID = ID++;
	}
}

void UActorIndexingComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

