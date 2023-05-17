#pragma once

#include <YC/Coroutine/Coro.hpp>
#include <YC/ErrorOr/ErrorOr.hpp>
#include <ranges>

#include "../GameCharacter/CharacterSystem.hpp"
#include "../Input/Input.hpp"
#include "../Log/Log.hpp"
#include "../NetPacket/PacketAction.hpp"
#include "../NetPacket/PacketTypes.h"
#include "../Static/ResourceMap.hpp"
#include "../Static/Static.hpp"
#include "../Utils/Functional.hpp"
#include "../Utils/String.hpp"
#include "../Web/Http.hpp"
#include "YC_UE_Template01/UI/UI.hpp"

namespace YC::Client {
struct FPacBuf {
	std::vector<Chanel_Player> PlayerPacs;
	void Clear() { PlayerPacs.clear(); }
};

struct FWorld {
	// Player //
	std::vector<System::Character::FPlayerData> Players;

	// Widget //
	ErrorOr<UUserWidget*> MainMenu = Err{std::string("Not Implemented")};
	ErrorOr<YcButton> StartButton = Err{std::string("Not Implemented")};
};

static FWorld Load(const UObject* G, FPacBuf* PacBuf, const ANetPC* NetPC) {
	const bool IsServer = (GetWorld(G) | GetNetPC | HasAuthority).Or(false);

	FPac_GetMyCharacterIndexFromServer::Bind([PacBuf](FPac_GetMyCharacterIndexFromServer Pac) {
		PacBuf->PlayerPacs.push_back(Pac);
	});
	FPac_SpawnedCharacterInServer::Bind([PacBuf](FPac_SpawnedCharacterInServer Pac) {
		PacBuf->PlayerPacs.push_back(Pac);
	});

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("%s : 피시 아이디 : %d"), (IsServer ? L"서버" : L"클라"), PC_ID % 2);
	const_cast<ANetPC*>(NetPC)->SetPCID(PC_ID % 2);
	PC_ID++;
#endif
	
	const ErrorOr<UUserWidget*> Widget = CreateWidget_(RES::WBP_MainMenu, const_cast<ANetPC*>(NetPC));
	Widget | AddToViewport | WhenErr | Log::Push_S;
	
	return {{}, {Widget.Unwrap()}, {Widget | GetButton("Button_2")} };
}

static ErrorOr<AActor*> FindNetEntity(const UObject* G, const int32& NetEntityIndex) {
	const auto Actors = GetWorld(G) | GetAllActors;
	if (Actors.IsErr()) return Err{std::string("GetAllActors is failed")};

	for (const auto Actor : Actors.Unwrap()) {
		if (const auto Index = Actor->FindComponentByClass<UActorIndexingComp>()) {
			const bool IsServer = (GetWorld(G) | GetNetPC | HasAuthority).Or(false);
			UE_LOG(LogTemp, Warning, TEXT("%s : FindNetEntity : %d == %d"), (IsServer ? L"서버" : L"클라"), Index->OwnID,
			       NetEntityIndex);
			if (Index->OwnID == NetEntityIndex) { return Actor; }
		}
	}

	return Err{std::string("FindNetEntity is failed")};
}

static System::Character::FPlayerData NewPlayerSetup(FPac_SpawnedCharacterInServer Packet) {
	return {
		.AChrPtr = Err{std::string("NewPlayerSetup : Not Implemented")},
		.CharacterData = System::Character::FOtherCharacterData{},
		.NetEntityIndex = Packet.NetEntityIndexOfServer,
		.PlayerType = static_cast<RES::EPlayer>(Packet.CharacterType),
	};
}

static System::Character::FPlayerData GetMyChr(FPac_GetMyCharacterIndexFromServer Packet) {
	auto R = NewPlayerSetup({Packet.NetEntityIndexOfServer, Packet.CharacterType});
	R.CharacterData = System::Character::FMyCharacterData{};
	return R;
}

static FWorld Loop(const UObject* G, const FPacBuf& PacBuf, FWorld World) {
	const auto NetPC = GetWorld(G) | GetNetPC;
	if (!NetPC.IsOk()) return World;
	const bool IsServer = (GetWorld(G) | GetNetPC | HasAuthority).Or(false);

	for (auto& Pac : PacBuf.PlayerPacs) {
		UE_LOG(LogTemp, Warning, TEXT("%s : 패킷 전송 확인!"), IsServer ? L"서버" : L"클라");
		const auto NewChr = Pac | NewPlayerSetup;
		const auto MyChr = Pac | GetMyChr;
		auto AddNewChr = [&](const System::Character::FPlayerData& Chr) { World.Players.push_back(Chr); };
		NewChr | AddNewChr;
		MyChr | AddNewChr;
	}
	// UI Update
	{
		World.StartButton | IsBtnUp | WhenOk | [&World, NetPC](auto R) {
			auto [NewBtn, Result] = R;
			World.StartButton = NewBtn;
			if (Result) { Send(FPac_SpawnAndPossess{RES::EPlayer::Padma}, NetPC.Unwrap()); }
		};
	}

	for (auto& [AChrPtr, CharacterData, NetEntityIndex, PlayerType] : World.Players) {

		auto UpdateChrData = [&CharacterData](auto Data){ CharacterData = Data; };
		
		if (AChrPtr.IsErr()) {
			AChrPtr = FindNetEntity(G, NetEntityIndex) | YC_Cast<ACharacter>;
			AChrPtr | WhenErr | Log::Push_S;
			if (AChrPtr.IsErr()) continue;
			CharacterData | [&](System::Character::FMyCharacterData Arg) {
				NetPC | GetSubsystem | AddMappingContext(RES::IMC_MainChr1, 0);
				NetPC | SetInputMode_GameOnly_;
				return Arg;
			} | WhenOk | UpdateChrData;
		}

		CharacterData = std::visit(Overload{
			                           [&](System::Character::FMyCharacterData& Arg) ->
			                           System::Character::FCharacterData {
				                           auto Input = NetPC | GetSubsystem | GetPlayerInput;
				                           Input | InputCheck(RES::IA_Move) | System::Character::Movement(
					                           NetPC.Unwrap(), AChrPtr.Unwrap());
				                           Input | InputCheck(RES::IA_Look) | System::Character::Look(AChrPtr.Unwrap())
					                           | WhenErr | Log::Push_S;
				                           return Arg;
			                           },
			                           [&](System::Character::FOtherCharacterData& Arg) ->
			                           System::Character::FCharacterData {
				                           return Arg;
			                           },
		                           }, CharacterData);
	}

	return World;
}
}
