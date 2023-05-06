#pragma once
#include "InputMappingContext.h"

namespace RES {

inline UInputMappingContext* IMC_MainChr1;
inline UClass* IMC_MainChr1_SC;

inline UInputAction* IA_Move;
inline UClass* IA_Move_SC;

inline USkeletalMesh* SKM_Manny;
inline UAnimBlueprint* ABP_Manny;

template <typename Type>
using ObFind = ConstructorHelpers::FObjectFinder<Type>;

inline std::vector<std::unordered_map<std::string, UClass*>> IA_SC_Map_List;

enum EPlayer {
	Padma,
	Feama,
	Num
};

inline void Load() {
	using Find_Imc = ObFind<UInputMappingContext>;
	using Find_IA = ObFind<UInputAction>;
	using Find_Skm = ObFind<USkeletalMesh>;
	using Find_ABP = ObFind<UAnimBlueprint>;
	
	IMC_MainChr1 = Find_Imc(L"/Game/Input/IMC_Chr1").Object;
	IMC_MainChr1_SC = IMC_MainChr1->StaticClass();
	
	IA_Move = Find_IA(L"/Game/Input/IA_Move").Object;
	IA_Move_SC = IA_Move->StaticClass();
	
	SKM_Manny =  Find_Skm(L"/Game/Characters/Mannequins/Meshes/SKM_Manny").Object;
	ABP_Manny = Find_ABP(L"/Game/Characters/Mannequins/Animations/ABP_Manny").Object;
	
	IA_SC_Map_List.resize(EPlayer::Num);
	
	IA_SC_Map_List[EPlayer::Padma]["IA_Move"] = IA_Move_SC;
	
}



}
