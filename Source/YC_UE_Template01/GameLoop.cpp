#include "GameLoop.h"

#include "GameWorld/GLoop.hpp"
#include "YC/Coroutine/Coro.hpp"

AGameLoop::AGameLoop() {
	PrimaryActorTick.bCanEverTick = true;

	RES::Load();
}

AGameLoop::~AGameLoop() {
	GameEnd();
}

void AGameLoop::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if(!StartFlag) {
		GameStart(this);
		Coro::CoStart([]() -> coroutine {
			while(YC_Global::IsGameLoopStarted) {
				co_yield wait_time { 1 };
				GameLoop();
			}
			co_return;
		}());
		StartFlag = true;
	}
	Coro::CoLoop();
}