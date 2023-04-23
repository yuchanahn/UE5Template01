#pragma once

#include <YC/ErrorOr/ErrorOr.hpp>

#include "GameFramework/Character.h"
#include "YC_UE_Template01/GameCharacter/CharacterSystem.hpp"
#include "YC_UE_Template01/Net/PacketAction.hpp"
#include "YC_UE_Template01/Net/Packets.hpp"

namespace YC {namespace System {namespace Controller {

inline std::vector<APlayerController*> Controllers;

inline ErrorOr<void> PossessTo(APlayerController* Controller, ACharacter* InChr) {
	if (IsValid(InChr)) {
		if(IsValid(Controller)) {
			Controller->Possess(InChr);
			return {};
		}
		return Err { std::string("Controller is not valid") };
	}
	return Err { std::string("Character is not valid") };
}

inline void Load(UObject* G, ANetPC* Controller) {
	//auto Chr = Character::CreateChr(G, Controller);
	//auto R1 = Chr | Curry(PossessTo)(Controller);
	//auto ChrData = Character::GetPlayerData(G);

	//Send(FTestPacket { 0, 0, L"Hello World" }, Controller);
	
	//if(Character::HandleError(R1)) return;
}

}}}
