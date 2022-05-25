#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "SpaceObject.hpp"

constexpr float FACTOR = 0.000001f;

constexpr int MOON_DISTANCE = 384400000;

constexpr float PLANET1_MASS = 1.972e25f;
constexpr float PlANET1_RADIUS = 6371000;
constexpr float PLANET2_MASS = 5.972e24f;
constexpr float PLANET2_RADIUS = 6371000;

#define absolutePos(x) ((x)/FACTOR)


int main(){
    std::ofstream file{"data.csv", std::ios::out};
    if(file.fail()){
		std::cout << "Log file failed to open\n";
        return EXIT_FAILURE;
	}
    file << "mvelX,mvelY,posX,posY,distance,i\n";
    int dataIndex = 0;

    sf::RenderWindow window(sf::VideoMode(2000, 1500), "lol");

    sf::Event event;
    sf::Clock logClock;
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



        if(logClock.getElapsedTime().asSeconds() > 0.1){
            file << planet2.mvelocity.x << ',' << planet2.mvelocity.y << ',' << planet2.getMPosition().x << ',' << planet2.getMPosition().y << ',' << planet2.distance(planet1) << ',' << dataIndex++ <<'\n';
            logClock.restart();
        }


        window.display();
    }

    return EXIT_SUCCESS;
}