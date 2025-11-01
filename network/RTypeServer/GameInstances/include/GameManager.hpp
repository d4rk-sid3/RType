#pragma once
#include "GameInstance.hpp"

class GameManager {
    private:
        std::map<std::string, std::shared_ptr<GameInstance>> active_games_;
        std::mutex mutex_;
        UdpServer& udp_server_; // Ajout de la référence au serveur UDP
    
    public:
        GameManager(UdpServer& udp_server);
        ~GameManager();
    
        std::shared_ptr<GameInstance> create_game(const std::string& id);
    
        std::shared_ptr<GameInstance> get_game(const std::string& id);
    
        void remove_game(const std::string& id);
    
        void list_games();
    };