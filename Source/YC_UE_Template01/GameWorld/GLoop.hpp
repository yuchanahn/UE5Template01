#pragma once
#include "YC/Time/Time.hpp"
#include "YC_UE_Template01/Controller/Controller.hpp"
#include "YC_UE_Template01/GameCharacter/CharacterSystem.hpp"

constexpr double DSec = 0.01;

namespace YC_Global {
inline bool IsGameLoopStarted = false;
inline std::chrono::time_point<std::chrono::steady_clock> GameLoopStartTime;
inline int TickCount;
inline UObject* GameMaster;
}

inline void GameStart(UObject* InGameMaster) {
	YC_Global::IsGameLoopStarted = true;
	YC_Global::GameLoopStartTime = std::chrono::high_resolution_clock::now();
	YC_Global::TickCount = 0;
	YC_Global::GameMaster = InGameMaster;
	
	YC::System::Character::Load(YC_Global::GameMaster);

	ANetPC* PC = Cast<ANetPC>(YC_Global::GameMaster->GetWorld()->GetFirstPlayerController());
	
	YC::System::Controller::Load(YC_Global::GameMaster, PC);
}

inline void GameEnd() { YC_Global::IsGameLoopStarted = false; }

inline void GameLoop() {
	const auto T = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - YC_Global::GameLoopStartTime).count() / 1000.0;
	
	double Sec = T - YC_Global::TickCount * DSec;
	while (Sec >= DSec) {
		// Main GameLoop
		// ...
		
		YC::System::Character::Loop();
		
		// End Main GameLoop
		Sec -= DSec;
		YC_Global::TickCount++;
	}
}