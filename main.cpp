#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "SpaceObject.hpp"

constexpr float FACTOR = 0.00005f;

constexpr float EARTH_MASS = 5.972e24f;
constexpr float EARTH_RADIUS = 6371000;

#define absolutePos(x) ((x)/FACTOR)

struct WindowSize{
    int width, height;
};
struct WindowSize getWindowSize(const int argc, const char** argv);

int main(const int argc, const char** argv){
    #ifdef logData
        std::ofstream file{"data.csv", std::ios::out};
        if(file.fail()){
	    	std::cout << "Log file failed to open\n";
            return EXIT_FAILURE;
	    }
        file << "mvelX,mvelY,posX,posY,distance,i\n";
        int dataIndex = 0;
    #endif

    auto [width, height] = getWindowSize(argc, argv);

    std::cout << "argc: " << argc << '\n';
    
    std::cout << "\e[?25l" << '\n'; //cursor off

    sf::RenderWindow window(sf::VideoMode(width, height), "lol", sf::Style::Titlebar | sf::Style::Close);

    sf::Event event;
    sf::Clock deltaClock;
    float dt;
    float passedTime = 0;

    SpaceObject earth{sf::Vector2f(absolutePos(750), absolutePos(500)), FACTOR, EARTH_RADIUS, sf::Color::Blue, EARTH_MASS};
    SpaceObject human{sf::Vector2f(absolutePos(750) + 1000000, absolutePos(500) + EARTH_RADIUS), FACTOR, 100000, sf::Color::White, 60};


    while(window.isOpen()){
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }

        dt = deltaClock.restart().asSeconds();
        // dt *= 100;


        human.Gvel(earth, dt);
        earth.Gvel(human, dt);

        human.moveMVel(dt);
        earth.moveMVel(dt);

        #ifdef logData
            if(logClock.getElapsedTime().asSeconds() > 0.1){
                file << planet2.mvelocity.x << ',' << planet2.mvelocity.y << ',' << planet2.getMPosition().x << ',' << planet2.getMPosition().y << ',' << planet2.distance(planet1) << ',' << dataIndex++ <<'\n';
                logClock.restart();
            }
        #endif

        // std::cout << "time passed: " << passedTime << '\r';
        std::cout << human << " time passed: " << passedTime << '\n';
        
        passedTime += dt;

        window.clear(sf::Color::Black);
        window.draw(earth);
        window.draw(human);
        window.display();
    }


    std::cout << "\e[?25h" << '\n'; //cursor on
    return EXIT_SUCCESS;
}

struct WindowSize getWindowSize(const int argc, const char** argv){

    struct WindowSize size = {0, 0};

    if(argc != 3){
        std::cout << "width: ";
        std::cin >> size.width;
        if(std::cin.fail()){
            std::cout << "Input failed" << '\n';
            exit(EXIT_FAILURE);
        }
        std::cout << "height: ";
        std::cin >> size.height;
        if(std::cin.fail()){
            std::cout << "Input failed" << '\n';
            exit(EXIT_FAILURE);
        }
    }
    else{
        size.width = std::atoi(argv[1]);
        size.height = std::atoi(argv[2]);
    }
    return size;
}