// ReSharper disable CppUE4CodingStandardNamingViolationWarning
#pragma once
#include <YC/ErrorOr/ErrorOr.hpp>

template <typename Type>
concept is_packet = requires {
	Type::__packet_id;
};

static int PC_ID = 0;
inline std::vector<std::function<void(const TArray<uint8>&)>> __packet_events[] = {
	std::vector<std::function<void(const TArray<uint8>&)>>(1000),
	std::vector<std::function<void(const TArray<uint8>&)>>(1000)
};
inline ErrorOr<void> __call_packet_event(const TArray<uint8>& data, const uint8_t packet_id, const int in_pc_id) {
	if(__packet_events[in_pc_id].size() <= packet_id) { return Err { std::string("Packet ID is out of range.") }; }
	if(!__packet_events[in_pc_id][packet_id]) { return Err { std::format("Packet ID({}) is not binded.", packet_id) }; }
	
	__packet_events[in_pc_id][packet_id](data);
	return {};
};

inline std::vector<std::function<void(const TArray<uint8>&, int)>> __server_packet_events(1000);
inline ErrorOr<void> __server__call_packet_event(const TArray<uint8>& data, const uint8_t packet_id, const int client_id) {
	if(__server_packet_events.size() <= packet_id) { return Err { std::string("Packet ID is out of range.") }; }
	if(!__server_packet_events[packet_id]) { return Err { std::string("Packet ID is not binded.") }; }
	
	__server_packet_events[packet_id](data, client_id);
	return {};
};