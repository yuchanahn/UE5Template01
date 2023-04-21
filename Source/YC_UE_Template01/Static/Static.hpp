#pragma once
#include "YC/ErrorOr/ErrorOr.hpp"

static ErrorOr<AActor*> Spawn(const TSubclassOf<AActor>& Class, const FVector& InLoc, const FRotator& InRot) {
	if (const auto World = GEngine->GetWorld()) {
		if (const auto Actor = World->SpawnActor<AActor>(Class, InLoc, InRot)) {
			return Actor;
		}
		return Err { std::string("SpawnActor failed!") };
	}
	return Err { std::string("GEngine->GetWorld() failed!") };
}
