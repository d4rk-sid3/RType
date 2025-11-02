#include "../include/ClientUI.hpp"
#include "../../Managers/ActionRegistry.hpp"
#include "../include/loader.hpp"

void ClientUI::loadPages()
{
    loadDynamicUI();
}

void ClientUI::init_date(void) {
    std::string date = getDate();
    auto dashboard_ui = UIManager::getInstance().getUI("Dashboardpage");
    auto today_date = std::make_shared<TextElement>("date", "assets_UI/fonts/Gabriela-Regular.ttf", date, sf::Color::White, (sf::Vector2f){588.0, 195.0}, 17);
    auto cast = std::dynamic_pointer_cast<UIElement>(today_date);
    dashboard_ui->addElement(cast);
    return;
}

void ClientUI::init_hour(void) {
    std::string date = getHour();
    auto dashboard_ui = UIManager::getInstance().getUI("Dashboardpage");
    hour = std::make_shared<TextElement>("Hour", "assets_UI/fonts/Gabriela-Regular.ttf", date, sf::Color::White, (sf::Vector2f){581.0, 151.0}, 27);
    auto cast = std::dynamic_pointer_cast<UIElement>(hour);
    dashboard_ui->addElement(cast);
    return;
}

void ClientUI::init(void) {
    ActionRegistry::getInstance().setDefaultActions();
    loadAllUI();
    UIManager::getInstance().setUI("Homepage");
    init_date();
    init_hour();
    window.setFramerateLimit(60);
}
ClientUI::~ClientUI(){};
ClientUI::ClientUI() : window(sf::VideoMode(738, 432), "R-Type") {};