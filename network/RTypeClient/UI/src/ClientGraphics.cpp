#include "../include/ClientGraphics.hpp"

ClientGraphics::ClientGraphics(std::shared_ptr<ThreadSafeQueue> queue) : _queue(queue) {
    clientUI.init();
}

void ClientGraphics::loadPages() {
    clientUI.loadPages();
}

void ClientGraphics::handleNetworkEvent(const std::string& line)
{
    std::string cmd = line;
    if (!cmd.empty() && (cmd.back() == '\r' || cmd.back() == '\n'))
        cmd.pop_back();

    std::istringstream iss(cmd);
    std::string keyword;
    std::vector<std::string> args;
    iss >> keyword;
    std::string token;

    while (iss >> token)
        args.push_back(token);

    if (keyword == "ERROR") {
        if (args.size() == 1) {
            std::cout << "ERROR" << std::endl;
            std::string error_message = "Please enter both your username and password to continue.";
            if (args[0] == "LOGIN") {
                auto currentUI = UIManager::getInstance().getUI("Loginpage");
                auto elem = currentUI->getElementById("message_login");
                auto cast = std::dynamic_pointer_cast<TextElement>(elem);
                cast->setText(error_message);
            } else if (args[0] == "REGISTER") {
                auto currentUI = UIManager::getInstance().getUI("Registerpage");
                auto elem = currentUI->getElementById("message_register");
                auto cast = std::dynamic_pointer_cast<TextElement>(elem);
                cast->setText(error_message);
            }
        }
    } else if (keyword == "REGISTER_OK") {
        std::string message = "Your account has been successfully created! You can now log in.";
        auto currentUI = UIManager::getInstance().getUI("Registerpage");
        auto username = currentUI->getElementById("username");
        auto cast_username = std::dynamic_pointer_cast<InputFieldElement>(username);
        cast_username->setText("");
        auto password = currentUI->getElementById("password");
        auto cast_password = std::dynamic_pointer_cast<InputFieldElement>(password);
        cast_password->setText("");
        auto elem = currentUI->getElementById("message_register");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);
        cast->setText(message);
    } else if (keyword == "LOGIN_FAIL") {
        auto currentUI = UIManager::getInstance().getUI("Loginpage");
        auto elem = currentUI->getElementById("message_login");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);
        cast->setText("The username or password you entered is not correct. Try again.");
    } else if (keyword == "LOGIN_OK") {
        std::string id = args[0];
        std::string username = args[1];
        std::string games_played = args[2];
        std::string games_won = args[3];
        std::string level = args[4];

        //Le username à gauche
        auto dashboardUI = UIManager::getInstance().getUI("Dashboardpage");
        auto elem_username = dashboardUI->getElementById("username_left");
        auto cast_username = std::dynamic_pointer_cast<TextElement>(elem_username);
        cast_username->setText(username);

        //Le username à droite
        auto right_username = dashboardUI->getElementById("username_right");
        auto cast_right = std::dynamic_pointer_cast<TextElement>(right_username);
        cast_right->setText(username);

        //Nombre de parties jouées
        auto games_playedd = dashboardUI->getElementById("games_played_number");
        auto cast_played = std::dynamic_pointer_cast<TextElement>(games_playedd);
        cast_played->setText(games_played);

        //Nombre de parties gagnées
        auto games_wonn = dashboardUI->getElementById("games_won_number");
        auto cast_won = std::dynamic_pointer_cast<TextElement>(games_wonn);
        cast_won->setText(games_won);

        //Level
        auto levell = dashboardUI->getElementById("level_number");
        auto cast_level = std::dynamic_pointer_cast<TextElement>(levell);
        cast_level->setText(level);

        UIManager::getInstance().setUI("Dashboardpage");
        return;
    }
}

ClientGraphics::~ClientGraphics() = default;

void ClientGraphics::run()
{
    std::string network_result = "";

    while (clientUI.window.isOpen()) {
        clientUI.deltaTime = clientUI.clock.restart().asSeconds();
        clientUI.eventHandler.pollEvents(clientUI.window);
        clientUI.window.clear();
        while (_queue->try_pop(network_result)) {
            handleNetworkEvent(network_result);
        }
        renderer.render(clientUI);
        clientUI.window.display();
    }
    return;
}