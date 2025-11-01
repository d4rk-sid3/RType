#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <mutex>
#include <vector>
#include <map>
#include <asio.hpp>
#include "../rtype_gameInstance/include/gameInstance.hpp"

class GameManager {
    private:
        std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& messages;

        std::mutex& mtx;

        std::map<std::string, std::shared_ptr<GameInstance>> active_games_;

        NetworkManager& udpServer;

        void process_messages();

        void addClientToGame(
            const std::string& game_id,
            asio::ip::udp::endpoint& client
        );
    
    public:
        GameManager(std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& messages, std::mutex& mtx, NetworkManager& _udpServer);

        ~GameManager();

        std::shared_ptr<GameInstance> create_game(const std::string& id);
        
        std::shared_ptr<GameInstance> get_game(const std::string& id);
        
        void remove_game(const std::string& id);
        
        void list_games();
};


#endif /* !GAME_MANAGER_HPP_ */