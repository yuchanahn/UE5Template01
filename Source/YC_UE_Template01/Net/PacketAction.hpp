#pragma once

#include "Packets.hpp"
#include "PRW.hpp"
#include "YC_UE_Template01/NetPC.h"


// Server 가 호출 합니다.
inline void Server_Send(is_packet auto Packet, ANetPC* Server) {
 	auto Data = YC::PRW::Serialize(Packet);
	
}

// Client 가 호출 합니다.
inline void Send(is_packet auto Packet, ANetPC* Client) {
	auto Data = YC::PRW::Serialize(Packet);
	Client->SendPacketToServer(Packet.__packet_id, Data);
}