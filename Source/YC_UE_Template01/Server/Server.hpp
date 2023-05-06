#pragma once
#include <ranges>
#include <YC/Coroutine/Coro.hpp>
#include <YC/ErrorOr/ErrorOr.hpp>

#include "YC_UE_Template01/NetPC.h"
#include "YC_UE_Template01/NetPacket/PacketAction.hpp"
#include "YC_UE_Template01/NetPacket/PacketTypes.h"


namespace YC { namespace Server {

constexpr int __MaxClientCount = 100;
inline int __ClientCount = 0;

struct ClientData {
	ANetPC* Ptr;
	int ID;
};

inline std::unordered_map<ANetPC*, ClientData> ClientList;
inline std::vector<ANetPC*> ClientPtrList(__MaxClientCount);
inline std::vector<System::Character::FPlayerData> ChrList(__MaxClientCount);

inline void OnClientConnect(ANetPC* Client) {
	ClientList[Client] = { Client, __ClientCount };
	ClientPtrList[__ClientCount++] = Client;
	
	//Server_Send(FPac_Test {1,2, std::format(L"테스트 중! 클라이언트 번호 : {}", __ClientCount).c_str() }, Client);
	//Server_Send(FPac_ClientConnected { "Hello Client!" }, Client);
}

inline void OnClientDisconnect(ANetPC* Client) {
	ClientPtrList[ClientList[Client].ID] = nullptr;
	ClientList.erase(Client);
}

inline ErrorOr<int> GetClientID(const ANetPC* Client) {
	if (Client == nullptr) return Err { std::string("Client is not valid") };
	if (!ClientList.contains(const_cast<ANetPC*>(Client))) return Err { std::string("!ClientList.contains(Client)") };
	
	return ClientList[const_cast<ANetPC*>(Client)].ID;
}

void Server_SendAll(is_packet auto Packet) {
	for (auto& [Ptr, ID] : ClientList | std::views::values) { Send(Packet, Ptr); }
}

inline void ServerLoad(UObject* G) {
	// 서버인 경우만 실행 해야 하는데...
	
	__ClientCount = 0;
	ChrList.clear();

	FPac_Test::ServerBind([](const FPac_Test& Packet, const int ClientId) {
		UE_LOG(LogTemp, Warning, TEXT("Server Recv - [%d]:%s"), ClientId, *Packet.ChatMassage);
		Server_Send(Packet, ClientPtrList[ClientId]);
	});
	
	FPac_SpawnAndPossess::ServerBind([G](const FPac_SpawnAndPossess& Packet, const int ClientId) {
		UE_LOG(LogTemp, Warning, TEXT("ClientSpawn Packet Recv"));
		const auto Chr = System::Character::SpawnCharacter_Padma(G);
		
		if(Chr.IsOk()){
			Chr.Unwrap().AChrPtr->SetReplicates(true);
			Chr.Unwrap().AChrPtr->SetReplicateMovement(true);
			
			ClientPtrList[ClientId]->Possess(Chr.Unwrap().AChrPtr);
			ChrList.push_back(Chr.Unwrap());
		}
	});
}

inline void ServerLoop() {
	// 클라이언트 스폰.

	// 클라이언트 Possess.

	// 몬스터 스폰.
	
	// 몬스터 로직.

	// 충돌 판정.

	// 액션?
	
}

}}

