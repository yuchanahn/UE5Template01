#pragma once
#include "YC/ErrorOr/ErrorOr.hpp"


namespace Mem {
inline ErrorOr<void> FreeU(UObject*& Ptr) {
	if (Ptr) {
		Ptr->ConditionalBeginDestroy();
		Ptr = nullptr;
		return {};
	}
	return Err { std::string("Ptr is nullptr!") };
}
}

namespace YC {

	static ErrorOr<APlayerController*> GetController(const ACharacter* InChr) {
		const auto PlayerController = InChr->GetController<APlayerController>();
		if (IsValid(PlayerController)) {
			return PlayerController;
		}
		return Err { std::string("PlayerController is not valid") };
	}

	static ErrorOr<UEnhancedInputLocalPlayerSubsystem*> GetSubsystem(const APlayerController* InContr) {
		const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InContr->GetLocalPlayer());
		if (IsValid(Subsystem)) {
			return Subsystem;
		}
		return Err { std::string("Subsystem is not valid") };
	}

	static ErrorOr<UEnhancedPlayerInput*> GetPlayerInput(const UEnhancedInputLocalPlayerSubsystem* InContr) {
		const auto InputComp = InContr->GetPlayerInput();
		if (IsValid(InputComp)) {
			return InputComp;
		}
		return Err { std::string("InputComp is not valid") };
	}
	static ErrorOr<USkeletalMeshComponent*> GetMesh(const ACharacter* InChr) {
		const auto Mesh = InChr->GetMesh();
		if (IsValid(Mesh)) {
			return Mesh;
		}
		return Err { std::string("Character Mesh is not valid") };
	}

	static ErrorOr<void> SetSkeletalMesh(USkeletalMeshComponent* InMeshComponent, USkeletalMesh* InMesh) {
		if (IsValid(InMeshComponent) && IsValid(InMesh)) {
			InMeshComponent->SetSkeletalMesh(InMesh);
			return {};
		}
		return Err { std::string("MeshComponent or Mesh is not valid") };
	}

	static ErrorOr<void> AddMappingContext(UInputMappingContext* InImc, int32 InPriority, UEnhancedInputLocalPlayerSubsystem* InSubSys) {
		if (IsValid(InSubSys) && IsValid(InImc)) {
			InSubSys->AddMappingContext(InImc, InPriority);
			return {};
		}
		return Err { std::string("Subsystem or InputMappingContext is not valid") };
	}
}


template <typename Type>
static ErrorOr<Type*> Spawn(const UObject* InObj, const TSubclassOf<Type>& Class, const FVector& InLoc, const FRotator& InRot) {
	const auto World = InObj->GetWorld();
	if (IsValid(World)) {
		auto Pram = FActorSpawnParameters{  };
		Pram.Name = FName("YC_FirstActor");
		const auto Actor = World->SpawnActor<Type>(Class, InLoc, InRot, Pram);
		if (IsValid(Actor)) {
			return Actor;
		}
		return Err { std::string("SpawnActor failed!") };
	}
	return Err { std::string("GEngine->GetWorld() failed!") };
}