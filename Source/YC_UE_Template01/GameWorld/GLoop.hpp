#pragma once
#include "YC/Time/Time.hpp"

constexpr double DSec = 0.01;

namespace YC_Global {
inline bool IsGameLoopStarted = false;
inline std::chrono::time_point<std::chrono::steady_clock> GameLoopStartTime;
inline int TickCount;
}

inline void GameStart() {
	YC_Global::IsGameLoopStarted = true;
	YC_Global::GameLoopStartTime = std::chrono::high_resolution_clock::now();
	YC_Global::TickCount = 0;
}

inline void GameEnd() { YC_Global::IsGameLoopStarted = false; }

inline void GameLoop() {
	const auto T = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - YC_Global::GameLoopStartTime).count() / 1000.0;
	double Sec = T - YC_Global::TickCount * DSec;
	while (Sec >= DSec) {
		// Main GameLoop
		// ...
		UE_LOG(LogTemp, Log, TEXT("GameLoop"));
		// End Main GameLoop
		Sec -= DSec;
		YC_Global::TickCount++;
	}
}