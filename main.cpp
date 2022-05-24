#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpaceObject.hpp"

constexpr float FACTOR = 0.000001f;

constexpr int MOON_DISTANCE = 384400000;

constexpr float PLANET1_MASS = 1.972e25f;
constexpr float PlANET1_RADIUS = 6371000;
constexpr float PLANET2_MASS = 5.972e24f;
constexpr float PLANET2_RADIUS = 6371000;



int main(){
    sf::RenderWindow window(sf::VideoMode(2000, 1500), "lol");

    sf::Event event;
    sf::Clock deltaClock;
    float dt;

    SpaceObject planet1{sf::Vector2f(500/FACTOR, 500/FACTOR), FACTOR, PLANET2_RADIUS, sf::Color::Blue, PLANET2_MASS};
    SpaceObject planet2{sf::Vector2f(500/FACTOR, 500/FACTOR + MOON_DISTANCE), FACTOR, PlANET1_RADIUS, sf::Color::White, PLANET1_MASS};


    planet2.mvelocity.x = 500;
    planet1.mvelocity.x = -500;

    while(window.isOpen()){
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }

        dt = deltaClock.restart().asSeconds();
        dt *= 100000;

        window.clear(sf::Color::Black);
        window.draw(planet1);
        window.draw(planet2);

        planet1.Gvel(planet2, dt);
        planet2.Gvel(planet1, dt);

        planet1.moveMVel(dt);
        planet2.moveMVel(dt);

        window.display();
    }

    return EXIT_SUCCESS;
}