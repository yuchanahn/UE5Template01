#pragma once
#include "YC_UE_Template01/GameCharacter/CharacterSystem.hpp"




namespace RES {

UInputMappingContext* IMC_MainChr1;

UInputAction* IA_Move;

USkeletalMesh* SKM_Manny;

template <typename Type>
using ObFind = ConstructorHelpers::FObjectFinder<Type>;

inline void Load() {
	using Find_Imc = ObFind<UInputMappingContext>;
	using Find_IA = ObFind<UInputAction>;
	using Find_Skm = ObFind<USkeletalMesh>;
	
	IMC_MainChr1 = Find_Imc(L"/Game/Input/IMC_Chr1").Object;

	IA_Move = Find_IA(L"/Game/Input/IA_Move").Object;

	SKM_Manny =  Find_Skm(L"/Game/Characters/Mannequins/Meshes/SKM_Manny").Object;

}
}