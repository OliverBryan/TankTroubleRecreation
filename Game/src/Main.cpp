#include "Environment.hpp"
#include "Resources.hpp"
#include "Config.hpp"
#include "StateManager.hpp"

#include <Log.hpp>

// TEMPORARY
class TestComponent : public gui::Component {
public:
    void render(sf::RenderWindow& window) const {
        sf::CircleShape cs(20.f);
        cs.setFillColor(sf::Color::Green);
        cs.setPosition(100.f, 100.f);
        window.draw(cs);
    }
    void tick() {}
};
// TEMPORARY

//TODO: don't crash when config file is not valid

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
    manager.setActiveState("game");

    // TEMPORARY
    manager.createState("test");
    manager.createComponentForState<TestComponent>("test");
    bool yPressed = false;
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
        }

        // update at a fixed rate (default 60tps)
        while (accumulator > sf::seconds(1.f / Environment::TPS)) {
            accumulator -= sf::seconds(1.f / Environment::TPS);

            // TEMPORARY
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && !yPressed) {
                yPressed = true;
                
                std::string newState = "";
                if (manager.getActiveState() == "game")
                    newState = "test";
                else newState = "game";

                manager.setActiveState(newState);
                Log::logStatus("Set game state to \"" + newState + "\"", ConsoleColor::DarkBlue);
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
                yPressed = false;
            // TEMPORARY

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