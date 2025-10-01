#include "../include/commands.hpp"

std::vector<uint8_t> encodeGamePausedRequest(const GamePausedRequest& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x33);
    return buffer;
}