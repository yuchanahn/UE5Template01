#pragma once
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "YC/ErrorOr/ErrorOr.hpp"
#include "YC/Curry/Curry.hpp"
#include "YC_UE_Template01/ActorIndexingComp.h"

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


struct FMyCharacterData {
	std::unordered_map<std::string, UInputAction*> IA_Map;
};
struct FOtherCharacterData {
	
};

using FCharacterData = std::variant<FMyCharacterData, FOtherCharacterData>;

struct FPlayerData {
	ACharacter* AChrPtr;
	FCharacterData CharacterData;
};

static ErrorOr<FPlayerData> SpawnCharacter_Padma(const UObject* GameMaster) {
	const auto Chr = Spawn<ACharacter>(GameMaster, ACharacter::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	
	Chr | GetMesh | Curry(SetSkeletalMesh, RES::SKM_Manny);
	Chr | GetMesh | Curry(SetRelativeLocation, FVector(0, 0, -88));
	Chr | GetMesh | Curry(SetRelativeRotation, FRotator(0, -90, 0));
	
	auto CameraBoom = Chr | YC_Cast<UObject>
						  | AddComp<USpringArmComponent>;

	auto ActorIndexing = Chr | YC_Cast<UObject>
							 | AddComp<UActorIndexingComp>
							 | RegisterComponent;
	//Chr.Unwrap()->AddComponent(FName("ActorIndexing"), true, FTransform::Identity, ActorIndexing.Unwrap());
	//ActorIndexing.Unwrap();
	
	const auto RootComp = Chr | GetRootComp;
	
	if(RootComp.IsErr()) return Err{RootComp.GetError()};
	
	CameraBoom | SetupAttachment(RootComp.Unwrap(), NAME_None);
	CameraBoom | SetTargetArmLength(600.0f);
	CameraBoom | SetUsePawnControlRotation(true);
	CameraBoom | RegisterComponent;
	
	auto FollowCamera = CameraBoom | AddComp<UCameraComponent>;
	
	if(CameraBoom.IsErr()) return Err{CameraBoom.GetError()};
	
	FollowCamera | SetupAttachment(CameraBoom.Unwrap(), USpringArmComponent::SocketName);
	FollowCamera | SetUsePawnControlRotation(false);
	FollowCamera | RegisterComponent;

	auto NewAction = [Chr](auto C) { return NewObject<UInputAction>(Chr.Unwrap(), C); };
	auto NewInputMapping = [Chr](auto C) { return NewObject<UInputMappingContext>(Chr.Unwrap(), C); };
	
	FPlayerData Data {};
	
	Data.AChrPtr = Chr.Unwrap();
	
	return Data;
}

static void Movement_(const APlayerController* PlayerController, ACharacter* InChr, const FInputActionValue& V) {
	const auto Axis = V.Get<FVector2D>();
	const auto YawRotation = FRotator(0, PlayerController->GetControlRotation().Yaw, 0);
	const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	InChr->AddMovementInput(ForwardDirection, Axis.Y);
	InChr->AddMovementInput(RightDirection, Axis.X);
}
static auto Movement = Curry(Movement_);

}}}
