#include "../include/GameInstance.hpp"

GameInstance::GameInstance(std::string &id, UdpServer& udp_server)
    : id_(std::move(id)), udp_server_(udp_server)
{
    std::cout << "Game " << id_ << " created." << std::endl;
}

GameInstance::~GameInstance(){}

void GameInstance::start() {
    //fonction principale de Oscar ici
}

void GameInstance::add_player(int client_id, const GamePlayer& player) {
    players_[client_id] = std::move(player);
    // Envoi d'une notification de confirmation au nouveau client via UDP
    send_game_state_to_players("Bienvenue dans le jeu !");
}

void GameInstance::game_loop(const boost::system::error_code& error) {
    //fonction de Oscar
}

void GameInstance::send_game_state_to_players(const std::string& message) {
    for (const auto& pair : players_) {
        // Utilise la méthode d'envoi du serveur UDP central
        udp_server_.send_to(message, pair.second);
    }
}