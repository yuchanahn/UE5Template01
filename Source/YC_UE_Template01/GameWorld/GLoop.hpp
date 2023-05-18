#pragma once

#include "GameCharacter/CharacterSystem.hpp"

#include "YC_UE_Template01/Client/Client.hpp"
#include "YC_UE_Template01/Server/Server.hpp"

constexpr double DSec = 0.01;

struct FGameStartData {
	std::chrono::time_point<std::chrono::steady_clock> GameLoopStartTime;
	YC::Client::FWorld World;
	YC::Client::FPacBuf* PacBuf;
};

inline FGameStartData GameStart(
	const UObject* InGameMaster,
	const std::vector<std::function<void()>>& OnGameStart_EventList
	) {
	Panic = [](const std::string& Msg) {
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString(Msg.c_str()));
		checkf(0, L"패닉! 스텍트레이스 읽어보삼!");
	};
	const auto PacBuf = new YC::Client::FPacBuf;
	const auto World = Load(InGameMaster, PacBuf, YC::GetNetPC(YC::GetWorld(InGameMaster).Unwrap()).Unwrap());
	
	// HasAuthority?
	if((YC::GetWorld(InGameMaster) | YC::GetNetPC | YC::HasAuthority).Or(false)) {
		YC::Server::ServerLoad(const_cast<UObject*>(InGameMaster));
	}

	for(auto& Event : OnGameStart_EventList) { Event(); }

	return {
		.GameLoopStartTime = std::chrono::high_resolution_clock::now(),
		.World = World,
		.PacBuf = PacBuf
	};
}

struct FGameLoopEndData {
	int TickCount;
	YC::Client::FWorld World;
	bool bIsGameEnd;
};

inline FGameLoopEndData GameLoop(
	const std::chrono::time_point<std::chrono::steady_clock> GameLoopStartTime,
	const int TickCount,
	const YC::Client::FPacBuf* PacBuf,
	const YC::Client::FWorld& World,
	const UObject* G
	) {
	const auto T = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - GameLoopStartTime).count() / 1000.0;
	
	int NewTickCount = TickCount;
	
	double Sec = T - TickCount * DSec;

	YC::Client::FWorld NewWorld = World;

	while (Sec >= DSec) {
		auto NetPc = YC::GetWorld(G) | YC::GetNetPC;

		if((NetPc | YC::HasAuthority).Or(false)) YC::Server::ServerLoop();
		
		if(NetPc.IsOk()) {
			const auto Pac_Clone = *PacBuf;
			const_cast<YC::Client::FPacBuf*>(PacBuf)->Clear();
			NewWorld = Tick(G, Pac_Clone, NewWorld);
		}
		Sec -= DSec;
		NewTickCount++;
	}

	NewWorld = UpdateWorld(G, NewWorld);
	
	return FGameLoopEndData {
		.TickCount = NewTickCount,
		.World = NewWorld,
		.bIsGameEnd = false
	};
}