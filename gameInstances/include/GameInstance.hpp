#pragma once
#include "../../Auth/include/librairies.hpp"
#include "./GamePlayer.hpp"

class GameInstance : public std::enable_shared_from_this<GameInstance> {
    private:
        std::string id_;
        UdpServer& udp_server_; // Référence au serveur UDP central (oscar)
        
        std::map<int, GamePlayer> players_;
    
    public:
        GameInstance(std::string &id, UdpServer& udp_server)
            : id_(std::move(id)), udp_server_(udp_server);

        ~GameInstance();
    
        void start();
    
        void add_player(int client_id, const GamePlayer& player);
        
        // Gère la logique de jeu (oscar)
        void game_loop(const boost::system::error_code& error);
    
        // Fonction utilisée pour envoyer des données à tous les joueurs
        void send_game_state_to_players(const std::string& message);
};