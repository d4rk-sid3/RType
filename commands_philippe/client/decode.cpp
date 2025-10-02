#include "../include/commands.hpp"

void decodeGamePausedRequest(const std::vector<uint8_t>& buffer)
{
    GamePausedRequest pos;

    if (buffer[0] != 0x33) {
        throw std::runtime_error("Invalid message type !");
    }
}