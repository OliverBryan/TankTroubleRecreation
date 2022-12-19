#include "Environment.hpp"
#include "Resources.hpp"
#include "Config.hpp"

#include <Log.hpp>

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

    // create the environment
    Environment env;

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

            env.tick();
            
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
        env.render(window);
        window.display();

        if (logFPS)
            frameCount++;

        // restart accumulator
        accumulator += clock.restart();
    }

    return 0;
}