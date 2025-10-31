#ifndef ACTIONREGISTRY
    #define ACTIONREGISTRY
    #include "../UI/include/librairies.hpp"
    #include "UIManager.hpp"
    #include "../Network/include/client.hpp"
    #include "../UI/components/include/InputFieldElement.hpp"

class ActionRegistry {
    public:
        using Action = std::function<void()>;
        std::unordered_map<std::string, Action> actions;
    
        static ActionRegistry& getInstance() {
            static ActionRegistry instance;
            return instance;
        }
    
        void registerAction(const std::string& name, Action action) {
            actions[name] = std::move(action);
        }
    
        Action get(const std::string& name) {
            auto it = actions.find(name);
            if (it != actions.end())
                return it->second;
            return nullptr;
        }

        void setDefaultActions() {
            const std::vector<std::string> pages = {
                "Homepage", "Registerpage", "Loginpage", "Dashboardpage", 
                "AboutUsPage", "HelpPage", "SettingsPage", "JoinParty"
            };
    
            for (const auto& page : pages) {
                registerAction(page, [page]() {
                    UIManager::getInstance().setUI(page);
                });
            }
        }

        void setNetworkActions(std::shared_ptr<Client> client) {
            registerAction("Login", [client]() {
                std::cout << "Loginnnnnn" << std::endl;
                auto ui = UIManager::getInstance().getCurrentUI();
                auto usernameField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("username"));
                auto passwordField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("password"));
    
                if (usernameField && passwordField) {
                    std::string username = usernameField->getText();
                    std::string password = passwordField->getText();
    
                    std::string message = "LOGIN " + username + " " + password + "\n";
                    client->write(message);
                }
            });
    
            registerAction("Register", [client]() {
                std::cout << "sjbjzkbhedvjz" << std::endl;
                auto ui = UIManager::getInstance().getCurrentUI();
                auto usernameField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("username"));
                auto passwordField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("password"));
    
                if (usernameField && passwordField) {
                    std::string username = usernameField->getText();
                    std::string password = passwordField->getText();
    
                    std::string message = "REGISTER " + username + " " + password + "\n";
                    client->write(message);
                }
            });
        }
    
};
#endif