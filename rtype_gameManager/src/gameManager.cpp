/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * level_parser *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄ * ▐
 * ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀ * █▄▄▄▄▄  ▐
 * █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             * █    ▌
 * █          █        █        █    ▌    █         █   █             * ▄▀▄▄▄▄
 * ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             * █    ▐
 * █         █       █ █          █    ▐   █     ▐   █   █               * ▐ ▐
 * ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#include "../include/gameManager.hpp"


GameManager::GameManager(
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& messages,
    std::mutex& mtx, NetworkManager& _udpServer) 
    : messages(messages), mtx(mtx), udpServer(_udpServer)
{
}

GameManager::~GameManager()
{
}

    
std::shared_ptr<GameInstance> GameManager::create_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (active_games_.count(id)) {
        return nullptr;
    }
    auto new_game = std::make_shared<GameInstance>(id, udpServer);
    active_games_[id] = new_game;
    
    return new_game;
}

std::shared_ptr<GameInstance> GameManager::get_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (active_games_.count(id)) {
        return active_games_[id];
    }
    return nullptr;
}

void GameManager::remove_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(mtx);
    active_games_.erase(id);
}

void GameManager::list_games() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Active Games: ";
    for (const auto& pair : active_games_) {
        std::cout << pair.first << " ";
    }
    std::cout << std::endl;
}

void GameManager::addClientToGame(
    const std::string& game_id,
    asio::ip::udp::endpoint& client
) {
    std::lock_guard<std::mutex> lock(mtx);
    if (active_games_.count(game_id)) {
        active_games_[game_id]->addClient(client);
    }
}

void GameManager::process_messages() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& msg_pair : messages) {
        const auto& endpoint = msg_pair.first;
        const auto& msg = msg_pair.second;

        for (auto& game_pair : active_games_) {
            auto& gameInstance = game_pair.second;
            if (gameInstance->hasClient(endpoint)) {
                gameInstance->addMessage(endpoint, msg);
                break;
            }
        }
    }
    messages.clear();
}