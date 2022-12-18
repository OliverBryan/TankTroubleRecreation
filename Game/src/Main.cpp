#include "Environment.hpp"

#include <box2d/box2d.h>

//TODO: exceptionpill

//#define DEBUG_LOG_FPS
#define VSYNC_ON

int main() {
    sf::ContextSettings cs; cs.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(1050, 670), "Tank Trouble", sf::Style::Titlebar | sf::Style::Close, cs); // Game space: 550 x 550

    Resources::init();

    Environment env;

    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;

    window.setFramerateLimit(144);

#ifdef VSYNC_ON
    window.setVerticalSyncEnabled(true);
#endif

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
            
#ifdef DEBUG_LOG_FPS
            counter++;

            if (counter > Environment::TPS) {
                counter = 0;
                std::cout << "FPS: " << frameCount << std::endl;
                frameCount = 0;
            }
#endif
        }

        window.clear(sf::Color::White);
        env.render(window);
        window.display();
#ifdef DEBUG_LOG_FPS
        frameCount++;
#endif DEBUG_LOG_FPS

        accumulator += clock.restart();
    }

    return 0;
}