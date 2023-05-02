#pragma once
#include "Packets.hpp"
#include "PRW.hpp"
#include "YC_UE_Template01/NetPC.h"

// Server 가 호출 합니다.
inline ErrorOr<void> Server_Send(is_packet auto Packet, const ANetPC* TargetClient) {
	if(TargetClient == nullptr) return Err { std::string("Target is not valid") };
	
 	auto Data = YC::PRW::Serialize(Packet);
	
	if(Data.Num() == 0) return Err { std::string("Serialize failed!") };

	TargetClient->SendPacketToClient(Packet.__packet_id, Data);
	return {};
}

// Client 가 호출 합니다.
inline ErrorOr<void> Send(is_packet auto Packet, const ANetPC* Client) {
	if(Client == nullptr) return Err { std::string("Client is not valid") };

	auto Data = YC::PRW::Serialize(Packet);

	if(Data.Num() == 0) return Err { std::string("Serialize failed!") };
	
	Client->SendPacketToServer(Packet.__packet_id, Data);
	return {};
}