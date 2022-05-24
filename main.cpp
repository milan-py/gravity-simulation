#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpaceObject.hpp"

constexpr float factor = 0.00004f;

constexpr float EARTH_GRAVITY_ACCELERATION = 9.81f;
constexpr float EARTH_MASS = 5.972e24;
constexpr float EARTH_RADIUS = 6371000;

int main(){
    sf::RenderWindow window(sf::VideoMode(2000, 1500), "lol");

    sf::Event event;
    sf::Clock deltaClock;
    float dt;

    SpaceObject earth{sf::Vector2f(1000/factor, 750/factor), factor, EARTH_RADIUS, sf::Color::Blue, EARTH_MASS};
    SpaceObject human{sf::Vector2f(1000/factor, 750/factor + EARTH_RADIUS + 10000), factor, 1000000, sf::Color{128, 0, 128}};

    human.mvelocity.x = 5000;

    while(window.isOpen()){
        try{
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }

        dt = deltaClock.restart().asSeconds();
        dt *= 100;

        window.clear(sf::Color::Black);
        window.draw(earth);
        window.draw(human);

        human.Gvel(earth, dt);



        human.moveMVel(dt);

        std::cout << human.mvelocity.x << " " << human.mvelocity.y << '\n';

        }
        catch(const std::exception& e){
            std::cerr << "Somethings fucked up: " << e.what() << '\n';
            
            return EXIT_FAILURE;
        }

        window.display();
    }

    return EXIT_SUCCESS;
}