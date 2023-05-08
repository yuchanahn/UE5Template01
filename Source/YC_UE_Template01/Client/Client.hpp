#pragma once

#include <YC/Coroutine/Coro.hpp>
#include <YC/ErrorOr/ErrorOr.hpp>

#include "YC_UE_Template01/GameCharacter/CharacterSystem.hpp"
#include "YC_UE_Template01/Input/Input.hpp"
#include "YC_UE_Template01/NetPacket/PacketAction.hpp"
#include "YC_UE_Template01/NetPacket/PacketTypes.h"
#include "YC_UE_Template01/Static/ResourceMap.hpp"
#include "YC_UE_Template01/Static/Static.hpp"
#include "YC_UE_Template01/Utils/Functional.hpp"
#include "YC_UE_Template01/Utils/String.hpp"

namespace YC { namespace Client {

static void SelectCharacter(const int8 CharacterID);

inline std::vector<Chanel_Player> PacOf_Player = {};
inline ErrorOr<ANetPC*> NetPC { "Not Setting" };
inline std::unordered_map<int32, UActorIndexingComp*> ANetEntity_List = {};
inline TMap<FString, std::function<void()>> UI_Call;

inline FString GetChatStringFrom(FPac_Test Packet) {
	//SelectCharacter(0);
	return { Packet.ChatMassage };
}

static void PrintUELog(const FString& Msg) {
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
}

static void Push_UEDisplayLog(FString Msg) {
	//GEngine->DisplayLog (LogTemp, Warning, TEXT("Client Recv - %s"), *Msg);
}
static void PrintUELog_S(const std::string& Msg) {
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(UTF8_TO_TCHAR(Msg.c_str())));
}

static void SendChatMessage(const FString& Msg) {
	Send(FPac_Test { 1, 2, Msg }, NetPC.Unwrap());
	//NetPC | Curry(Send)(FPac_Test { 1, 2, Msg });
}

static void SelectCharacter(const int8 CharacterID) {
	Send(FPac_SpawnAndPossess { CharacterID }, NetPC.Unwrap());
	//Send();
}

static System::Character::FPlayerData Setup_CharacterInput(
	const ANetPC* PC,
	const System::Character::FPlayerData& MyPlayer,
	const UClass* IMC_SC,
	const std::unordered_map<std::string, UClass*>& IA_SC_List
	) {
	
	GetSubsystem(PC) | AddMappingContext(NewObject<UInputMappingContext>(MyPlayer.AChrPtr, IMC_SC), 0);
	
	auto Result_Player = MyPlayer;
	
	auto Data = std::get<System::Character::FMyCharacterData>(Result_Player.CharacterData);
	for(const auto& [fst, snd] : IA_SC_List) {
		 Data.IA_Map[fst] = NewObject<UInputAction>(MyPlayer.AChrPtr, snd);
	}
	
	Result_Player.CharacterData = Data;
	return Result_Player;
}

static ErrorOr<int32> GetMyChrNumber(const FPac_GetMyCharacterIndexFromServer& Packet) {
	UE_LOG(LogTemp, Warning, TEXT("MyChrIndex : %d"), Packet.CharacterIndexOfServer);
	return Packet.CharacterIndexOfServer;
}

static ErrorOr<System::Character::FPlayerData> GetPlayerData(const int32 CharacterIndexOfServer) {
	if(ANetEntity_List.contains(CharacterIndexOfServer)) {
		const auto Owner = ANetEntity_List[CharacterIndexOfServer]->GetOwner();
		if(Owner == nullptr) {
			return Err { std::string("Not Found Owner") };
		}
		const auto Chr = Cast<ACharacter>(Owner);
		if(Chr == nullptr) {
			return Err { std::string("Not Found ACharacter") };
		}
		
		return System::Character::FPlayerData { Chr };
	}
	return Err { std::string("Not Found Character index of Array") };
}

static void Load(const UObject* G) {
	Coro::CoStart([](const UObject* G) -> coroutine {
		NetPC = GetWorld(G) | GetNetPC;
		while(!NetPC.IsOk()) {
			co_yield wait_time { 1 };
			NetPC = GetWorld(G) | GetNetPC;
		}
		FPac_Test::Bind([](FPac_Test Pac){ PacOf_Player.push_back(Pac); });
		FPac_GetMyCharacterIndexFromServer::Bind([](FPac_GetMyCharacterIndexFromServer Pac){ PacOf_Player.push_back(Pac); });

		Send(FPac_Test { 1, 2, "Hello Server!" }, NetPC.Unwrap());
		ErrorOr<UUserWidget*> Widget = NetPC | CreateWidget(RES::WBP_MainMenu);
		Widget | AddToViewport | WhenErr | PrintUELog_S;
	}(G));
	
	UI_Call.Add(L"SpawnActor", [] {
		//auto f =  [NetPC = NetPC.Unwrap()](auto Packet){ return Send(Packet, NetPC); };
		//f(FPac_Test { 1, 2, "SpawnActor" });
		SelectCharacter(RES::EPlayer::Padma);
	});
}

static void Loop() {
	static ErrorOr<int32> MyChrIndex = Err { std::string("Not Setup MyPlayer") };
	ErrorOr<System::Character::FPlayerData> MyChr = Err { std::string("Not Setup MyPlayer") };
	for(auto& Pac : PacOf_Player) {
		Pac | GetChatStringFrom | AppendFront_FStr(L"서버 메시지 : ") | PrintUELog;
		MyChrIndex = Pac | GetMyChrNumber;
		MyChr = MyChrIndex | GetPlayerData;

		UE_LOG(LogTemp, Warning, TEXT("패킷 왔음!"));
		//| Setup_CharacterInput;
	}
	if(MyChr.IsOk()) {
		const auto Input = NetPC | GetSubsystem
								 | GetPlayerInput;
		
		Input | WhenErr | PrintUELog_S;
		//Input | std::bind_front(InputCheck_, MyChr.Unwrap().CharacterData) | WhenErr | PrintUELog_S;
	}
	
	PacOf_Player.clear();
}

static void Unload() {
	
}

}}
