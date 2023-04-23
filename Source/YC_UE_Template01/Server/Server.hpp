#pragma once

#include <YC/ErrorOr/ErrorOr.hpp>

#include "YC_UE_Template01/Net/Packets.hpp"


inline void ServerLoad() {
	FTestPacket::ServerBind([](const FTestPacket& Packet, const int ClientId) {
		UE_LOG(LogTemp, Warning, TEXT("Server Recv - [%d]:%s"), ClientId, *Packet.ChatMassage);
	});
}

inline void ServerLoop() {
	
}