#pragma once
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "YC/ErrorOr/ErrorOr.hpp"
#include "YC_UE_Template01/NetPC.h"


class UCameraComponent;

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

	static ErrorOr<void> SetSkeletalMesh( USkeletalMesh* InMesh, USkeletalMeshComponent* InMeshComponent) {
		if (IsValid(InMeshComponent) && IsValid(InMesh)) {
			InMeshComponent->SetSkeletalMesh(InMesh);
			return {};
		}
		return Err { std::string("MeshComponent or Mesh is not valid") };
	}

	static ErrorOr<void> SetRelativeLocation(const FVector& InLocation, USkeletalMeshComponent* InMeshComponent) {
		if (IsValid(InMeshComponent)) {
			InMeshComponent->SetRelativeLocation(InLocation);
			return {};
		}
		return Err { std::string("MeshComponent is not valid") };
	}

	static ErrorOr<void> SetRelativeRotation(const FRotator& InRotation, USkeletalMeshComponent* InMeshComponent) {
		if (IsValid(InMeshComponent)) {
			InMeshComponent->SetRelativeRotation(InRotation);
			return {};
		}
		return Err { std::string("MeshComponent is not valid") };
	}


	static ErrorOr<void> SetAnimInstance(UClass* InAnimInstance, USkeletalMeshComponent* InMeshComponent) {
		if (IsValid(InMeshComponent) && IsValid(InAnimInstance)) {
			InMeshComponent->SetAnimInstanceClass(InAnimInstance);
			return {};
		}
		return Err { std::string("MeshComponent or AnimInstance is not valid") };
	}

	static ErrorOr<UInputAction*> NewIA_(UClass*& C, UObject* InInputComp) {
		if (IsValid(InInputComp) && IsValid(C)) {
			return NewObject<UInputAction>(InInputComp, C);
		}
		return Err { std::string(!IsValid(C) ? "StaticClass is not valid" : "InputComp is not valid") };
	}
	static auto NewIA = Curry(NewIA_);

	static ErrorOr<void> AddMappingContext_(const UInputMappingContext* InImc, const int32 InPriority, UEnhancedInputLocalPlayerSubsystem* InSubSys) {
		if (IsValid(InSubSys) && IsValid(InImc)) {
			InSubSys->AddMappingContext(InImc, InPriority);
			return {};
		}
		return Err { std::string("Subsystem or InputMappingContext is not valid") };
	}
	static auto AddMappingContext = Curry(AddMappingContext_);

	static ErrorOr<UWorld*> GetWorld(const UObject* InObj) {
		if(!IsValid(InObj)) return Err { std::string("InObj is not valid") };
		UWorld* World = nullptr;
		try {
			World = InObj->GetWorld();
			if (IsValid(World)) {
				return World;
			}
		}
		catch (const std::exception&) {
			return Err { std::string("GetWorld() failed!") };
		}
		return Err { std::string("World is not valid") };
	}

	static ErrorOr<ANetPC*> GetNetPC(const UWorld* InWorld) {
		if(!IsValid(InWorld)) return Err { std::string("InWorld is not valid") };
		if(!IsValid(InWorld->GetFirstPlayerController())) return Err { std::string("InWorld->GetFirstPlayerController() is not valid") };
		const auto NetPC = Cast<ANetPC>(InWorld->GetFirstPlayerController());
		if (IsValid(NetPC)) {
			return NetPC;
		}
		return Err { std::string("NetPC is not valid") };
	}

	static ErrorOr<bool> HasAuthority(const ANetPC* InNetPC) {
		if (IsValid(InNetPC)) {
			return InNetPC->HasAuthority();
		}
		return Err { std::string("InNetPC is not valid") };
	}

	template <typename Type>
	static ErrorOr<Type*> AddComp(UObject* InObj) {
		if (IsValid(InObj)) {
			if(auto R = NewObject<Type>(InObj, Type::StaticClass())) {
				return R;
			}
			return Err { std::string("NewObject<Type> is Failed") };
		}
		return Err { std::string("InObj is not valid") };
	}
	
	template <typename Type>
	static ErrorOr<Type*> YC_Cast(UObject* InObj) {
		if (IsValid(InObj)) {
			if(auto R = Cast<Type>(InObj)) {
				return R;
			}
			return Err { std::string("Cast<Type> is Failed") };
		}
		return Err { std::string("InObj is not valid") };
	}

	static ErrorOr<USceneComponent*> GetRootComp(const AActor* InActor) {
		if (IsValid(InActor)) {
			return InActor->GetRootComponent();
		}
		return Err { std::string("InActor is not valid") };
	}

	static ErrorOr<void> SetupAttachment_(USceneComponent* InParent, FName InSocketName, USceneComponent* InComp) {
		if (IsValid(InComp) && IsValid(InParent)) {
			InComp->SetupAttachment(InParent, InSocketName);
			return {};
		}
		return Err { std::string("InChild or InParent is not valid") };
	}

	inline auto SetupAttachment = Curry(SetupAttachment_);

	static ErrorOr<void> SetUsePawnControlRotation_(bool InUse, UCameraComponent* InComp) {
		if (IsValid(InComp)) {
			InComp->bUsePawnControlRotation = InUse;
			return {};
		}
		return Err { std::string("InComp is not valid") };
	}
	static ErrorOr<void> SetUsePawnControlRotation_2(bool InUse, USpringArmComponent* InComp) {
		if (IsValid(InComp)) {
			InComp->bUsePawnControlRotation = InUse;
			return {};
		}
		return Err { std::string("InComp is not valid") };
	}
	inline auto SetUsePawnControlRotation = Curry(SetUsePawnControlRotation_);
	inline auto SetUsePawnControlRotation_SpringArm = Curry(SetUsePawnControlRotation_2);

	static ErrorOr<void> SetTargetArmLength_(const float InLength, USpringArmComponent* InComp) {
		if (IsValid(InComp)) {
			InComp->TargetArmLength = InLength;
			return {};
		}
		return Err { std::string("InComp is not valid") };
	}
	inline auto SetTargetArmLength = Curry(SetTargetArmLength_);

	static ErrorOr<UActorComponent*> RegisterComponent(UActorComponent* InComp) {
		if (IsValid(InComp)) {
			InComp->RegisterComponent();
			return InComp;
		}
		return Err { std::string("InComp is not valid") };
	}

	static void PrintError (is_t_err_opt auto InErr) {
		if (InErr.IsErr()) {
			UE_LOG(LogTemp, Error, TEXT("%s"), InErr.GetError().c_str());
		}
	}

	static ErrorOr<UUserWidget*> CreateWidget_(const TSubclassOf<UUserWidget> C, APlayerController* InObj) {
		if (IsValid(InObj) && IsValid(C)) {
			return CreateWidget<UUserWidget>(InObj, C);
		}
		return Err { std::string("InObj or C is not valid") };
	}
	inline auto CreateWidget = Curry(CreateWidget_);

	static ErrorOr<void> AddToViewport(UUserWidget* InWidget) {
		if (IsValid(InWidget)) {
			InWidget->AddToViewport();
			return {};
		}
		return Err { std::string("InWidget is not valid") };
	}

	static ErrorOr<UActorIndexingComp*> SetIndex_For_ActorIndexingComp_(const int32 InIndex, UActorIndexingComp* InComp) {
		if (IsValid(InComp)) {
			InComp->OwnID = InIndex;
			return InComp;
		}
		return Err { std::string("InComp is not valid") };
	}
	inline auto SetIndex_For_ActorIndexingComp = Curry(SetIndex_For_ActorIndexingComp_);
}

template <typename Type>
static ErrorOr<Type*> Spawn(const UObject* InObj, const TSubclassOf<Type>& Class, const FVector& InLoc, const FRotator& InRot) {
	const auto World = InObj->GetWorld();
	if (IsValid(World)) {
		auto Pram = FActorSpawnParameters{  };
		const auto Actor = World->SpawnActor<Type>(Class, InLoc, InRot, Pram);
		if (IsValid(Actor)) {
			return Actor;
		}
		return Err { std::string("SpawnActor failed!") };
	}
	return Err { std::string("GEngine->GetWorld() failed!") };
}