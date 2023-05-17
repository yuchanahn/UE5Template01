#pragma once
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "YC/ErrorOr/ErrorOr.hpp"
#include "YC/Curry/Curry.hpp"
#include "YC_UE_Template01/ActorIndexingComp.h"
#include "YC_UE_Template01/Log/Log.hpp"

#include "YC_UE_Template01/Static/ResourceMap.hpp"
#include "YC_UE_Template01/Static/Static.hpp"
#include "YC_UE_Template01/Utils/Functional.hpp"
#include "YC_UE_Template01/Utils/String.hpp"

namespace YC::System::Character {

struct FMyCharacterData {
};
struct FOtherCharacterData {
	
};

using FCharacterData = std::variant<FMyCharacterData, FOtherCharacterData>;

struct FPlayerData {
	ErrorOr<ACharacter*> AChrPtr = Err{ std::string("AChrPtr is not valid") };
	FCharacterData CharacterData;
	int32 NetEntityIndex;
	RES::EPlayer PlayerType;
};

static ErrorOr<void> SetChrSKMAndABP(USkeletalMesh* Skm, UClass* Abp, const ACharacter* Chr) {
	auto Err1 = GetMesh(Chr) | Curry(SetSkeletalMesh, Skm),
	     Err2 = GetMesh(Chr) | Curry(SetAnimInstance, Abp);
	
	if(Err1.IsErr()) return Err{Err1.GetError()};
	if(Err2.IsErr()) return Err{Err2.GetError()};
	
	return {};
}

static ErrorOr<FPlayerData> SpawnPlayer(const UObject* GameMaster, const int32 NetEntityIndex) {
	
	const auto Tr = FTransform {FRotator::ZeroRotator, FVector::ZeroVector};
	const auto Chr = SpawnActorDef<ACharacter>(GameMaster, RES::BP_Chr1, Tr);
	Chr | FinishSpawningActor(Tr) | WhenErr | Log::Push_S;
	
	auto ActorIndexing = Chr | YC_Cast<UObject>
		| AddComp<UActorIndexingComp>
		| SetIndex_For_ActorIndexingComp(NetEntityIndex)
		| RegisterComponent;
	
	if(Chr.IsErr()) return Err{ Chr.GetError() };
	
	return FPlayerData {
		.AChrPtr = Chr.Unwrap(),
		.CharacterData = FOtherCharacterData {},
		.NetEntityIndex = NetEntityIndex,
		.PlayerType = RES::EPlayer::Padma
	};
}

static ErrorOr<void> SetChrCam(ACharacter* Chr) {
	auto CameraBoom = YC_Cast<UObject>(Chr) | AddComp<USpringArmComponent>;
	
	const auto RootComp = GetRootComp(Chr);
	
	if(RootComp.IsErr()) return Err{RootComp.GetError()};
	
	CameraBoom | SetupAttachment(RootComp.Unwrap(), NAME_None);
	CameraBoom | SetTargetArmLength(600.0f);
	CameraBoom | SetUsePawnControlRotation_SpringArm(true);
	CameraBoom | RegisterComponent;
	
	auto FollowCamera = CameraBoom | AddComp<UCameraComponent>;
	
	if(CameraBoom.IsErr()) return Err{CameraBoom.GetError()};
	
	FollowCamera | SetupAttachment(CameraBoom.Unwrap(), USpringArmComponent::SocketName);
	FollowCamera | SetUsePawnControlRotation(false);
	FollowCamera | RegisterComponent;
	
	return {};
}

static void Movement_(const APlayerController* PlayerController, ACharacter* InChr, const FInputActionValue& V) {
	const auto Axis = V.Get<FVector2D>();
	const auto YawRotation = FRotator(0, PlayerController->GetControlRotation().Yaw, 0);
	const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	InChr->AddMovementInput(ForwardDirection, Axis.Y);
	InChr->AddMovementInput(RightDirection, Axis.X);
}
//Movement_(const APlayerController*, ACharacter*, const FInputActionValue&) -> void
static auto Movement = Curry(Movement_);

static void Look_(ACharacter* InChr, const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	InChr->AddControllerYawInput(-LookAxisVector.X);
	InChr->AddControllerPitchInput(LookAxisVector.Y);
}
//Look_(const FInputActionValue&) -> void
static auto Look = Curry(Look_);


}
