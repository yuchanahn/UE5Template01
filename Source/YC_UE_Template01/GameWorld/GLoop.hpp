#pragma once
#include "YC_UE_Template01/Client/Client.hpp"
#include "YC_UE_Template01/Controller/Controller.hpp"
#include "YC_UE_Template01/GameCharacter/CharacterSystem.hpp"
#include "YC_UE_Template01/Server/Server.hpp"

constexpr double DSec = 0.01;

namespace YC_Global {
inline bool IsGameLoopStarted = false;
inline std::chrono::time_point<std::chrono::steady_clock> GameLoopStartTime;
inline int TickCount;
inline UObject* GameMaster;
inline std::vector<std::function<void()>> OnGameStart_EventList;
}

inline void GameStart(UObject* InGameMaster) {

	Panic = [](const std::string& Msg) {
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString(Msg.c_str()));
		checkf(0, L"패닉! 스텍트레이스 읽어보삼!");
	};
	
	YC_Global::IsGameLoopStarted = true;
	YC_Global::GameLoopStartTime = std::chrono::high_resolution_clock::now();
	YC_Global::TickCount = 0;
	YC_Global::GameMaster = InGameMaster;

	YC::Client::Load(YC_Global::GameMaster);

	// HasAuthority?
	if((YC::GetWorld(YC_Global::GameMaster) | YC::GetNetPC | YC::HasAuthority).Or(false)) {
		YC::Server::ServerLoad(YC_Global::GameMaster);
	}

	for(auto& Event : YC_Global::OnGameStart_EventList) { Event(); }
	YC_Global::OnGameStart_EventList.clear();
	//YC::System::Character::Load(YC_Global::GameMaster);
}

inline void GameEnd() {
	YC_Global::IsGameLoopStarted = false;
	YC::Client::Unload();
}

inline void GameLoop() {
	const auto T = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - YC_Global::GameLoopStartTime).count() / 1000.0;
	
	double Sec = T - YC_Global::TickCount * DSec;
	while (Sec >= DSec) {
		// Main GameLoop
		// ...
		auto NetPc = YC::GetWorld(YC_Global::GameMaster) | YC::GetNetPC;
		
		if((NetPc | YC::HasAuthority).Or(false)) {
			YC::Server::ServerLoop();
		}
		
		NetPc | WhenOk
			  | YC::Client::Loop;
		
		// End Main GameLoop
		Sec -= DSec;
		YC_Global::TickCount++;
	}
}