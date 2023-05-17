#include "GameLoop.h"

#include "GameWorld/GLoop.hpp"
#include "YC/Coroutine/Coro.hpp"

AGameLoop::AGameLoop() {
	PrimaryActorTick.bCanEverTick = true;
	RES::Load();
}

void AGameLoop::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if(GetWorld()->GetFirstPlayerController() == nullptr) return;
	
	std::call_once(StartFlag, [&] {
		auto [StartTime, World, PacBuf] = GameStart(this, {});
		this->StartTime = StartTime;
		this->PacBuf = PacBuf;
		this->World = World;
	});
	
	auto [Tick, NewWorld, GameEnded] = GameLoop(StartTime, TickCount, PacBuf, World, this);
	
	TickCount = Tick;
	World = NewWorld;
	
	Coro::CoLoop();
}

void AGameLoop::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if(this->PacBuf) {
		delete this->PacBuf;
		this->PacBuf = nullptr;
	}
}
