#include "../include/GameManager.hpp"

GameManager::GameManager(UdpServer& udp_server) : udp_server_(udp_server) {}
        
GameManager::~GameManager();
    
std::shared_ptr<GameInstance> GameManager::create_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (active_games_.count(id)) {
        return nullptr;
    }            
    auto new_game = std::make_shared<GameInstance>(id, udp_server_);
    active_games_[id] = new_game;
    
    return new_game;
}

std::shared_ptr<GameInstance> GameManager::get_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (active_games_.count(id)) {
        return active_games_[id];
    }
    return nullptr;
}

void GameManager::remove_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    active_games_.erase(id);
}

void GameManager::list_games() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "Active Games: ";
    for (const auto& pair : active_games_) {
        std::cout << pair.first << " ";
    }
    std::cout << std::endl;
}