#include "Environment.hpp"
#include "Resources.hpp"
#include "Config.hpp"
#include "StateManager.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "ColorFilter.hpp"

#include <Log.hpp>

// TODO: don't crash when config file is not valid
// TODO: some ui bugs with buttons, effects, and state changes (i.e. pause game)

int main() {
    // initiate log and load config settings
    Log::init("Log.txt");
    Config::init();

    // check settings
    const bool vsyncEnabled = Config::getSetting("enableVSync", true);
    const bool logFPS = Config::getSetting("logFPS", false);

    Log::logStatus(std::string("VSync is ") + (vsyncEnabled ? "enabled" : "disabled"), ConsoleColor::LightPurple);

    // create window
    sf::ContextSettings cs; cs.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(1050, 670), "Tank Trouble", sf::Style::Titlebar | sf::Style::Close, cs); // Game space: 550 x 550

    // load sprites
    Resources::init();

    gui::StateManager manager;
    manager.createState("game");
    manager.createComponentForState<Environment>("game");

    // TEMPORARY
    manager.createState("menu");
    sf::Vector2f size(200.f, 100.f);

    gui::EffectState initialState {size, sf::Color(52, 235, 134)};
    gui::EffectState hoverState {size * 1.1f, sf::Color(54, 191, 115)};

    gui::Effect hover(initialState, hoverState, sf::milliseconds(50));
    gui::Effect click(hoverState, {size, sf::Color(36, 133, 79)}, sf::milliseconds(50));

    manager.createComponentForState<gui::Button>("menu", sf::Vector2f(525.f, 335.f), size, 10.f, sf::Color(52, 235, 134), sf::Color::Black, "Play", 36, hover, click, [&manager]() {
        manager.setActiveState("game");
    });
    manager.createComponentForState<gui::Label>("menu", sf::Vector2f(525.f, 200.f), 50, "Tank Trouble");
 
    manager.createState("pause");
    manager.createComponentForState<gui::ColorFilter>("pause", sf::Color(255, 255, 255, 150));
    manager.createComponentForState<gui::Button>("pause", sf::Vector2f(525.f, 335.f), size, 10.f, sf::Color(52, 235, 134), sf::Color::Black, "Resume", 36, hover, click, [&manager]() {
        manager.setActiveState("game");
        manager.setBackgroundState("");
    });
    manager.createKeyTransition("game", "pause", sf::Keyboard::Escape, true);

    manager.setActiveState("menu");
    // TEMPORARY

    // clock and accumulator to keep a fixed speed update loop
    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;

    // vsync
    if (vsyncEnabled)
        window.setVerticalSyncEnabled(true);

    // for debug logging
    std::size_t frameCount = 0;
    std::size_t counter = 0;

    // main loop
    while (window.isOpen()) {
        sf::Event event;

        // check events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            manager.handleEvent(event);
        }

        // update at a fixed rate (default 60tps)
        while (accumulator > sf::seconds(1.f / Environment::TPS)) {
            accumulator -= sf::seconds(1.f / Environment::TPS);

            manager.tick();
            
            // debug logging
            if (logFPS) {
                counter++;

                if (counter > Environment::TPS) {
                    counter = 0;
                    Log::logStatus("FPS: " + std::to_string(frameCount), ConsoleColor::NeonBlue);
                    frameCount = 0;
                }
            }
        }

        // render
        window.clear(sf::Color::White);
        manager.render(window);
        window.display();

        if (logFPS)
            frameCount++;

        // restart accumulator
        accumulator += clock.restart();
    }

    return 0;
}