#include "NetPC.h"

#include "Net/Packets.hpp"

void ANetPC::SendPacketToServer(const uint8 ID, const TArray<uint8>& Packet) {
	S_ServerRecvPacket(ID, Packet);
}
void ANetPC::SendPacketToClient(const TArray<uint8>& Packet, int ClientId) {
	
}

void ANetPC::S_ServerRecvPacket_Implementation(uint8 ID, const TArray<uint8>& Packet) {
	NMC_ServerRecvPacket_Implementation(ID, Packet);
}
void ANetPC::NMC_ServerRecvPacket_Implementation(uint8 ID, const TArray<uint8>& Packet) {
	__server__call_packet_event(Packet, ID, 0/*ThisClientID*/);
}