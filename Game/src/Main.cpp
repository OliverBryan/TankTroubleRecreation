#include "Environment.hpp"

#include <box2d/box2d.h>
#include <Log.hpp>

#include "Config.hpp"

//TODO: don't crash when reading config file

int main() {
    Log::init("Log.txt");

    Config::init();

    const bool vsyncEnabled = Config::getSetting("enableVSync", true);
    const bool logFPS = Config::getSetting("logFPS", false);

    Log::logStatus(std::string("VSync is ") + (vsyncEnabled ? "enabled" : "disabled"), ConsoleColor::LightPurple);

    sf::ContextSettings cs; cs.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(1050, 670), "Tank Trouble", sf::Style::Titlebar | sf::Style::Close, cs); // Game space: 550 x 550

    Resources::init();

    Environment env;

    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;

    window.setFramerateLimit(144);

    if (vsyncEnabled)
        window.setVerticalSyncEnabled(true);

    std::size_t frameCount = 0;
    std::size_t counter = 0;
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        while (accumulator > sf::seconds(1.f / Environment::TPS)) {
            accumulator -= sf::seconds(1.f / Environment::TPS);

            env.tick();
            
            if (logFPS) {
                counter++;

                if (counter > Environment::TPS) {
                    counter = 0;
                    Log::logStatus("FPS: " + std::to_string(frameCount), ConsoleColor::NeonBlue);
                    frameCount = 0;
                }
            }
        }

        window.clear(sf::Color::White);
        env.render(window);
        window.display();

        if (logFPS)
            frameCount++;

        accumulator += clock.restart();
    }

    return 0;
}