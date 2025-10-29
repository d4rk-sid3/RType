#ifndef ACTIONREGISTRY
    #define ACTIONREGISTRY
    #include "../UI/include/librairies.hpp"
    #include "UIManager.hpp"

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
                "Settings", "Scoreboard", "CreateParty", "JoinParty"
            };
    
            for (const auto& page : pages) {
                registerAction(page, [page]() {
                    UIManager::getInstance().setUI(page);
                });
            }
        }
};
#endif