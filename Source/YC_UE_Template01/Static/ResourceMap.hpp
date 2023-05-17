#pragma once
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"

namespace RES {

inline UInputMappingContext* IMC_MainChr1;
inline UInputAction* IA_Move;
inline UInputAction* IA_Attack;
inline UInputAction* IA_Look;

inline USkeletalMesh* SKM_Manny;
inline TSubclassOf<UAnimInstance> ABP_Manny;
inline TSubclassOf<UUserWidget> WBP_MainMenu;
inline TSubclassOf<ACharacter> BP_Chr1;

template <typename Type>
using ObFind = ConstructorHelpers::FObjectFinder<Type>;

template <typename Type>
using CFind = ConstructorHelpers::FClassFinder<Type>;

enum EPlayer {
	Padma,
	Feama,
	Num
};


inline void Load() {
	using Find_Imc = ObFind<UInputMappingContext>;
	using Find_IA =  ObFind<UInputAction>;
	using Find_Skm = ObFind<USkeletalMesh>;
	using Find_ABP = CFind<UAnimInstance>;
	using Find_WBP = CFind<UUserWidget>;
	using Find_Character = CFind<ACharacter>;
	
	IMC_MainChr1 = Find_Imc(L"/Game/Input/IMC_Chr1").Object;
	
	IA_Move = Find_IA(L"/Game/Input/IA_Move").Object;
	IA_Attack = Find_IA(L"/Game/Input/IA_Attack").Object;
	IA_Look = Find_IA(L"/Game/Input/IA_Look").Object;
	
	SKM_Manny =  Find_Skm(L"/Game/Characters/Mannequins/Meshes/SKM_Manny").Object;
	
	ABP_Manny = Find_ABP(L"/Game/Characters/Mannequins/Animations/ABP_Manny").Class;
	WBP_MainMenu = Find_WBP(L"/Game/UI/NewWidgetBlueprint").Class;
	BP_Chr1 = Find_Character(L"/Game/Characters/MyTEST_Chr").Class;
}

}
