#pragma once
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "YC/ErrorOr/ErrorOr.hpp"
#include "YC/Curry/Curry.hpp"

#include "YC_UE_Template01/Input/Input.hpp"
#include "YC_UE_Template01/Static/ResourceMap.hpp"
#include "YC_UE_Template01/Static/Static.hpp"
#include "YC_UE_Template01/Utils/String.hpp"

namespace YC {namespace System {namespace Character {
template <typename ErrorOrType>
bool HandleError(const ErrorOrType& Result) {
	if (Result.IsErr()) {
		UE_LOG(LogTemp, Error, L"%s", *ToFStr(Result.GetError().c_str()));
		return true;
	}
	return false;
}

struct FCharacterData {
	// Input Setting
	UInputAction* IA_Move;
};

inline std::vector<ACharacter*> Chrs;
inline std::vector<FCharacterData> ChrDataList;

inline void Load(UObject* GameMaster) {
	Chrs.clear();
	ChrDataList.clear();

	const auto MainPlayer = Spawn<ACharacter>(GameMaster, ACharacter::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

	FCharacterData Data;
	auto NewAction = [GameMaster](auto C) { return NewObject<UInputAction>(GameMaster, C); };
	auto NewInputMapping = [GameMaster](auto C) { return NewObject<UInputMappingContext>(GameMaster, C); };


	Data.IA_Move = NewAction(RES::IA_Move_SC);

	const auto ResultSkm = MainPlayer | GetMesh | Curry(SetSkeletalMesh)(RES::SKM_Manny);
	const auto ResultImc = MainPlayer | GetController | GetSubsystem | Curry(AddMappingContext)(NewInputMapping(RES::IMC_MainChr1_SC), 0);

	if (HandleError(ResultSkm) || HandleError(ResultImc)) return;

	MainPlayer.Expect();

	ChrDataList.push_back(Data);
	Chrs.push_back(MainPlayer.Unwrap());
}

static void Movement(const APlayerController* PlayerController, ACharacter* InChr, const FInputActionValue& V) {
	const auto Axis = V.Get<FVector2D>();
	const auto YawRotation = FRotator(0, PlayerController->GetControlRotation().Yaw, 0);
	const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	InChr->AddMovementInput(ForwardDirection, Axis.Y);
	InChr->AddMovementInput(RightDirection, Axis.X);
}

inline void Loop() {
	for (auto& Chr : Chrs) {
		const auto Controller = GetController(Chr);
		const auto PlayerInput = Controller | GetSubsystem | GetPlayerInput;
		if (HandleError(PlayerInput)) return;
		if (HandleError(InputCheck(PlayerInput.Unwrap(), ChrDataList[0].IA_Move) | Curry(Movement)(Controller.Unwrap(), Chr))) return;
	}
}

inline void Unload() {
	Chrs.clear();
	ChrDataList.clear();
}
}}}
