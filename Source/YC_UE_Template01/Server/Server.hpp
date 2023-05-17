#pragma once
#include <ranges>
#include <YC/ErrorOr/ErrorOr.hpp>

#include "YC_UE_Template01/NetPC.h"
#include "YC_UE_Template01/NetPacket/PacketAction.hpp"
#include "YC_UE_Template01/NetPacket/PacketTypes.h"


namespace YC { namespace Server {

constexpr int __MaxClientCount = 100;

struct ClientData {
	ANetPC* Ptr;
	int ID;
};

inline std::unordered_map<ANetPC*, ClientData> ClientList;
inline std::vector<ANetPC*> ClientPtrList(__MaxClientCount);
inline std::vector<System::Character::FPlayerData> ChrList(__MaxClientCount);

inline void OnClientConnect(ANetPC* Client) {
	ClientList[Client] = { Client, static_cast<int>(ClientList.size()) };
	ClientPtrList[ClientList.size() - 1] = Client;
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
	for (auto& [Ptr, ID] : ClientList | std::views::values) {
		Server_Send(Packet, Ptr) | WhenErr | YC::Log::Push_S;
	}
}

void Server_SendAllBut(is_packet auto Packet, const ANetPC* Client) {
	for (auto& [Ptr, ID] : ClientList | std::views::values) {
		if(Ptr != Client) {
			Server_Send(Packet, Ptr) | WhenErr | YC::Log::Push_S;
		}
	}
}
inline int32 GetNetEntityID(bool Reset = false) {
	static int32 ID = 0;
	if(Reset) ID = 0;
	return ID++;
}

inline void ServerLoad(UObject* G) {
	// 서버인 경우만 실행 해야 하는데...
	GetNetEntityID(true);
	ChrList.clear();

	FPac_SpawnAndPossess::ServerBind([G](const FPac_SpawnAndPossess& Packet, const int ClientId) {
		const auto Chr = System::Character::SpawnPlayer(G, GetNetEntityID());
		Chr | WhenErr | YC::Log::Push_S;
		if(Chr.IsOk()){
			Chr.Unwrap().AChrPtr.Unwrap()->SetReplicates(true);
			Chr.Unwrap().AChrPtr.Unwrap()->SetReplicateMovement(true);
			Chr.Unwrap().AChrPtr.Unwrap()->GetMesh()->SetIsReplicated(true);

			
			ClientPtrList[ClientId]->Possess(Chr.Unwrap().AChrPtr.Unwrap());
			ChrList.push_back(Chr.Unwrap());
			UE_LOG(LogTemp, Warning, TEXT("To Client ID : %d"), ClientId);
			
			Server_SendAllBut(FPac_SpawnedCharacterInServer { Chr.Unwrap().NetEntityIndex, RES::EPlayer::Padma }, ClientPtrList[ClientId]);
			Server_Send(FPac_GetMyCharacterIndexFromServer { Chr.Unwrap().NetEntityIndex, RES::EPlayer::Padma }, ClientPtrList[ClientId]);
		}
	});
	
	//Coro::CoStart([]() -> coroutine {
	//while(true) {
	//	co_yield wait_time { 1000 };
	//	for(auto Clnt : ClientList){
	//		UE_LOG( LogTemp, Warning, TEXT("{%d , %p}"), Clnt.second.ID, Clnt.second.Ptr);
	//	}
	//}
	//}());
}

inline void ServerLoop() {

}

}}

