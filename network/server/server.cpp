#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <ctime>
#include <sodium.h> // libsodium
#include "server.hpp"

TokenManager* TokenManager::s_pInstance = nullptr;
UserManager* UserManager::s_pInstance = nullptr;

int main() {
    // === 2. Création d'une clé serveur (32 bytes aléatoires) ===
    uint8_t server_key[32];
    randombytes_buf(server_key, sizeof(server_key));

    // === 4. Création de users ===
    UserManager::Instance()->createUser("Alice", "password123");
    UserManager::Instance()->createUser("Bob", "secret456");

    std::cout << "=== Test authentification ===" << std::endl;

    // === 5. Authentification avec mot de passe correct ===
    if (UserManager::Instance()->authenticate("Alice", "password123", server_key)) {
        std::cout << "Alice authenticated successfully!" << std::endl;

        // Affichage tokens
        auto aliceOpt = UserManager::Instance()->getUserByUsername("Alice");
        if (aliceOpt) {
            User& alice = aliceOpt.value().get();   // récupère la référence réelle
            std::cout << "Auth token (hex): " << alice.getAuthTokenHex() << std::endl;
        }
    } else {
        std::cout << "Alice failed authentication!" << std::endl;
    }

    // === 6. Authentification avec mot de passe incorrect ===
    if (UserManager::Instance()->authenticate("Bob", "wrongpass", server_key)) {
        std::cout << "Bob authenticated successfully!" << std::endl;
    } else {
        std::cout << "Bob failed authentication!" << std::endl;
    }

    // === 7. Authentification avec mot de passe correct ===
    if (UserManager::Instance()->authenticate("Bob", "secret456", server_key)) {
        std::cout << "Bob authenticated successfully!" << std::endl;
        auto bobOpt = UserManager::Instance()->getUserByUsername("Bob");
        if (bobOpt) {
            User& bob = bobOpt.value().get();
            std::cout << "Auth token (hex): " << bob.getAuthTokenHex() << std::endl;
            //std::cout << "Session token (hex): " << bob->getSessionTokenHex() << std::endl;
        }
    }

    return 0;
}
