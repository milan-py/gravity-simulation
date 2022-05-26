#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "SpaceObject.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

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
    
    std::cout << "\e[?25l" << '\n'; //cursor off

    sf::RenderWindow window(sf::VideoMode(width, height), "lol", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    if(!ImGui::SFML::Init(window)){
        std::cout << "imgui failed!" << '\n';
        return EXIT_FAILURE;
    }

    sf::Event event;
    sf::Clock imguiClock;
    sf::Clock deltaClock;
    float dt;
    float passedTime = 0;

    SpaceObject earth{sf::Vector2f(absolutePos(750), absolutePos(500)), FACTOR, EARTH_RADIUS, sf::Color::Blue, EARTH_MASS};
    SpaceObject human{sf::Vector2f(absolutePos(750), absolutePos(500) + EARTH_RADIUS + 100000), FACTOR, 200000, sf::Color::White, 60};

    human.mvelocity.x = 7600;


    while(window.isOpen()){
        while(window.pollEvent(event)){
            ImGui::SFML::ProcessEvent(window, event);
            if(event.type == sf::Event::Closed or event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
                window.close();
            }
        }

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
        
        
        ImGui::SFML::Update(window, imguiClock.restart());
        dt = deltaClock.restart().asSeconds();
        std::cout << "deltatime: " << dt << " estimated fps: " << 1/dt << '\r';
        dt *= 1000;
        passedTime += dt;
        
        if(earth.ispressed(window)){
            earth.showWindow = !earth.showWindow;
        }
        if(human.ispressed(window)){
            human.showWindow = !human.showWindow;
        }
        earth.window("earth");
        human.window("human");

        window.clear(sf::Color::Black);
        window.draw(earth);
        window.draw(human);
        ImGui::SFML::Render(window);
        window.display();
    }


    std::cout << "\e[?25h" << '\n'; //cursor on
    return EXIT_SUCCESS;
}

struct WindowSize getWindowSize(const int argc, const char** argv){

    struct WindowSize size = {0, 0};

    if(argc < 3){
        size.width = 1500;
        size.height = 1000;
    }
    else{
        size.width = std::atoi(argv[1]);
        size.height = std::atoi(argv[2]);
    }
    return size;
}