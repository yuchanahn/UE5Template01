#pragma once
#include "PRW.hpp"

template <typename Type>
concept is_packet = requires {
	Type::__packet_id;
};

inline std::vector<std::function<void(const TArray<uint8>&, int)>> __packet_events(1000);
inline auto __call_packet_event(const TArray<uint8>& data, uint8_t packet_id, int client_id) {
	__packet_events[packet_id](data, client_id);
};

inline std::vector<std::function<void(const TArray<uint8>&, int)>> __server_packet_events(1000);
inline auto __server__call_packet_event(const TArray<uint8>& data, uint8_t packet_id, int client_id) {
	__server_packet_events[packet_id](data, client_id);
};