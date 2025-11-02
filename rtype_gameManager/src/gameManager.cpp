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


GameManager& GameManager::getInstance() {
    static GameManager instance; 
    return instance;
}

void GameManager::init(
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& messages,
    std::mutex& mtx,
    NetworkManager& _udpServer
) {
    GameManager& instance = getInstance();
    
    if (instance.messages_ != nullptr) {
        return; 
    }
    
    instance.messages_ = &messages;
    instance.mtx_ = &mtx;
    instance.udpServer_ = &_udpServer;
}

// --- ACCESSEURS ---

static void check_init(const void* ptr, const std::string& componentName) {
    if (ptr == nullptr) {
        throw std::runtime_error("GameManager not initialized. Call GameManager::init() before accessing " + componentName + ".");
    }
}

std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& GameManager::getMessages() {
    check_init(messages_, "messages");
    return *messages_;
}

std::mutex& GameManager::getMutex() {
    check_init(mtx_, "mutex");
    return *mtx_;
}

NetworkManager& GameManager::getUdpServer() {
    check_init(udpServer_, "UDP server");
    return *udpServer_;
}

GameManager::~GameManager() {
    for (auto& thread : gameThreads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

// --- LOGIQUE DE JEU ---

std::shared_ptr<GameInstance> GameManager::create_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(getMutex()); 
    
    if (active_games_.count(id)) {
        return nullptr;
    }
    auto new_game = std::make_shared<GameInstance>(id, getUdpServer()); 
    std::cout << "Game created with ID: " << id << std::endl;
    active_games_[id] = new_game;
    
    return new_game;
}

std::shared_ptr<GameInstance> GameManager::get_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(getMutex());
    if (active_games_.count(id)) {
        return active_games_[id];
    }
    return nullptr;
}

void GameManager::remove_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(getMutex());
    active_games_.erase(id);
}

void GameManager::list_games() {
    std::lock_guard<std::mutex> lock(getMutex());
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
    std::lock_guard<std::mutex> lock(getMutex());
    if (active_games_.count(game_id)) {
        active_games_[game_id]->addClient(client);
    }
}

void GameManager::process_messages() {
    std::lock_guard<std::mutex> lock(getMutex());
    for (auto& msg_pair : getMessages()) { 
        const auto& endpoint = msg_pair.first;
        const auto& msg = msg_pair.second;

        for (auto& game_pair : active_games_) {
            auto& gameInstance = game_pair.second;
            if (gameInstance->hasClient(endpoint)) {
                std::cout << "Processing message for game: " << game_pair.first << std::endl;
                gameInstance->addMessage(endpoint, msg);
                break;
            }
        }
    }
    getMessages().clear();
}

void GameManager::start_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(getMutex());
    if (active_games_.count(id)) {
        std::cout << "Starting game with ID: " << id << std::endl;

        std::thread gameThread([this, id]() {
            active_games_[id]->run();
        });
        gameThreads_.emplace_back(std::move(gameThread));
    }
}