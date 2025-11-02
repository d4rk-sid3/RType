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
    std::cout << keyword << std::endl;

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
    } else if (keyword == "BANNED") {
        auto currentUI = UIManager::getInstance().getUI("Loginpage");
        auto elem = currentUI->getElementById("message_login");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);
        cast->setText("You have been banned from the server. Contact the administrator.");
    }
    else if (keyword == "LOGIN_OK") {
        std::string id = args[0];
        std::string username = args[1];
        std::string games_played = args[2];
        std::string games_won = args[3];
        std::string level = args[4];
        std::string up = args[5];
        std::string down = args[6];
        std::string left = args[7];
        std::string right = args[8];
        std::string space = args[9];

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

        //Je get l'interface settings
        auto settings_ui = UIManager::getInstance().getUI("SettingsPage");
        
        //Je recupere les elements un a un et je set les input fields
        //UP
        auto input_up = settings_ui->getElementById("input_up");
        auto input_up_cast = std::dynamic_pointer_cast<InputFieldElement>(input_up);
        input_up_cast->setText(up);

        //DOWN
        auto input_down = settings_ui->getElementById("input_down");
        auto input_down_cast = std::dynamic_pointer_cast<InputFieldElement>(input_down);
        input_down_cast->setText(down);

        //LEFT
        auto input_left = settings_ui->getElementById("input_left");
        auto input_left_cast = std::dynamic_pointer_cast<InputFieldElement>(input_left);
        input_left_cast->setText(left);

        //RIGHT
        auto input_right = settings_ui->getElementById("input_right");
        auto input_right_cast = std::dynamic_pointer_cast<InputFieldElement>(input_right);
        input_right_cast->setText(right);

        //SHOOT
        auto input_shoot = settings_ui->getElementById("shoot");
        auto input_shoot_cast = std::dynamic_pointer_cast<InputFieldElement>(input_shoot);
        input_shoot_cast->setText(space);

        UIManager::getInstance().setUI("Dashboardpage");
        return;
    } else if (keyword == "SAVE_OK") {
        std::cout << "SAVE_OK" << std::endl;
        auto settings_ui = UIManager::getInstance().getCurrentUI();
        auto message_element = settings_ui->getElementById("message_settings");
        auto cast_element = std::dynamic_pointer_cast<TextElement>(message_element);

        cast_element->setText("Your informations have successfully been updated.");
    } else if (keyword == "CODE") {
        std::string code = args[0];

        auto create_ui = UIManager::getInstance().getUI("CreateParty");
        auto message_element = create_ui->getElementById("code_value");
        auto cast_element = std::dynamic_pointer_cast<TextElement>(message_element);

        cast_element->setText(code);
        UIManager::getInstance().setUI("CreateParty");
    } else if (keyword == "ADMIN_OK") {
        std::cout << "Administrator authentication successful. Closing the graphical interface.\n";
        
        _isAdmin = true; 
        clientUI.window.close();
        return;
    }
}

ClientGraphics::~ClientGraphics() = default;

bool ClientGraphics::isAdmin() {
    return _isAdmin;
}

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