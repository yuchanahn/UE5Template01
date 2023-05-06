#include "NetPC.h"

#include "GameWorld/GLoop.hpp"
#include "NetPacket/Packets.hpp"
#include "Server/Server.hpp"
#include "Utils/String.hpp"


void ANetPC::BeginPlay() {
	Super::BeginPlay();
	if(HasAuthority()) {
		if(YC_Global::IsGameLoopStarted) {
			YC::Server::OnClientConnect(this);
		} else {
			YC_Global::OnGameStart_EventList.push_back([this]() {
				YC::Server::OnClientConnect(this);
			});
		}
	}
}

void ANetPC::Destroyed() {
	Super::Destroyed();
	if(HasAuthority()) {
		YC::Server::OnClientDisconnect(this);
	}
}

void ANetPC::SendPacketToServer(const uint8 ID, const TArray<uint8>& Packet) const {
	S_ServerRecvPacket(ID, Packet);
}

void ANetPC::SendPacketToClient(const uint8 ID, const TArray<uint8>& Packet) const {
	C_ClientRecvPacket(ID, Packet);
}

void ANetPC::UI_MsgCall(const FString Msg) {
	if(YC::Client::UI_Call.Contains(Msg)) {
		YC::Client::UI_Call[Msg]();
	}
}

void ANetPC::C_ClientRecvPacket_Implementation(const uint8 ID, const TArray<uint8>& Packet) const{
	const auto Err = __call_packet_event(Packet, ID);
	if(Err.IsErr()) {
		UE_LOG(LogTemp, Error, TEXT("Packet Error: %s"), *ToFStr(Err.GetError()));
	}
}

void ANetPC::S_ServerRecvPacket_Implementation(const uint8 ID, const TArray<uint8>& Packet) const{
	auto ClntID = YC::Server::GetClientID(this);
	if(ClntID.IsErr()) {
		UE_LOG(LogTemp, Error, TEXT("Client ID is not valid!"));
		return;
	}
	const auto Err = __server__call_packet_event(Packet, ID, ClntID.Unwrap());
	if(Err.IsErr()) {
		UE_LOG(LogTemp, Error, TEXT("Packet Error: %s"), *ToFStr(Err.GetError()));
	}
}
