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
struct WindowSize readWindowSize(const int argc, const char** argv);
void UpdateCameraVelocity(sf::Vector2f &cameraVelocity);


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

    auto [width, height] = readWindowSize(argc, argv);
    
    std::cout << "\e[?25l" << '\n'; //cursor off

    sf::RenderWindow window(sf::VideoMode(width, height), "lol", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);
    if(!ImGui::SFML::Init(window)){
        std::cout << "imgui failed!" << '\n';
        return EXIT_FAILURE;
    }

    sf::Vector2i viewpos{0, 0};
    sf::View view = window.getDefaultView();

    sf::Event event;
    sf::Clock imguiClock;
    sf::Clock deltaClock;
    float dt;
    float passedTime = 0;
    float timeFactor = 1;

    sf::Vector2f cameraVelocity{0, 0};

    std::vector<SpaceObject> spaceObjects{
        SpaceObject("earth1", sf::Vector2(absolutePos(width/2)+EARTH_RADIUS*2, absolutePos(height/2)), FACTOR, EARTH_RADIUS, sf::Color::Blue, EARTH_MASS),
        SpaceObject("earth2", sf::Vector2(absolutePos(width/2), absolutePos(height/2)), FACTOR, EARTH_RADIUS, sf::Color::Blue, EARTH_MASS),
    };

    // human.mvelocity.x = 7600;

    while(window.isOpen()){ // main loop
        while(window.pollEvent(event)){
            ImGui::SFML::ProcessEvent(window, event);
            if(event.type == sf::Event::Closed or (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape)){
                window.close();
            }
            if(event.type == sf::Event::Resized){
                view = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
            }
            
        }
        

        std::cout << "x: " << cameraVelocity.x << " y: " << cameraVelocity.y << '\n';
        UpdateCameraVelocity(cameraVelocity);
        view.move(cameraVelocity);

        /*human.Gvel(earth, dt);
        earth.Gvel(human, dt);*/


        for(SpaceObject &i: spaceObjects){
            i.moveMVel(dt);
            for(SpaceObject &j: spaceObjects){
                if(!i.isme(j)){
                    i.Gvel(j, dt);
                }
            }
        }

        #ifdef logData
            if(logClock.getElapsedTime().asSeconds() > 0.1){
                file << planet2.mvelocity.x << ',' << planet2.mvelocity.y << ',' << planet2.getMPosition().x << ',' << planet2.getMPosition().y << ',' << planet2.distance(planet1) << ',' << dataIndex++ <<'\n';
                logClock.restart();
            }
        #endif
        
        
        ImGui::SFML::Update(window, imguiClock.restart());
        dt = deltaClock.restart().asSeconds();

        //imgui menu bar 
        if(ImGui::BeginMainMenuBar()){
            if(ImGui::BeginMenu("file")){
                if(ImGui::MenuItem("close window", "ctrl + esc")){
                    window.close();
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            ImGui::Text("seconds passed: %.3f", passedTime);
            ImGui::SliderFloat("", &timeFactor, 0, 10000, "%.2f");
            if(ImGui::Button("1x")){
                timeFactor = 1;
            }
            ImGui::Text("time factor");
            ImGui::EndMainMenuBar();
        }
        dt *= timeFactor;
        passedTime += dt;
    

        window.clear(sf::Color::Black);
        for(SpaceObject &i: spaceObjects){
            if(i.ispressed(window)){
                i.showWindow = !i.showWindow;
            }
            i.window();
            window.draw(i);
        }

        std::cout << spaceObjects[0] << '\n';

        // SFML screen update
        
        ImGui::SFML::Render(window);
        window.display();
        window.setView(view);
    }


    std::cout << "\e[?25h" << '\n'; //cursor on
    return EXIT_SUCCESS;
}

void UpdateCameraVelocity(sf::Vector2f &cameraVelocity){
    
    // handling the keyboard input for the camera movement
    // TODO: add mouse dragging

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        if(cameraVelocity.y <= 4){
            // std::cout << "lol";
            cameraVelocity.y += 0.1;
        }   
    }
    else if(not sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
        if(cameraVelocity.y > 0){
            cameraVelocity.y -= 0.2;
        }
        else{
            cameraVelocity.y = 0;
        }
    } 
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
        if(cameraVelocity.y >= -4){
            // std::cout << "lol";
            cameraVelocity.y -= 0.1;
        }   
    }
    else if(not sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        if(cameraVelocity.y < 0){
            cameraVelocity.y += 0.2;
        }
        else{
            cameraVelocity.y = 0;
        }
    }  


    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        if(cameraVelocity.x <= 4){
            // std::cout << "lol";
            cameraVelocity.x += 0.1;
        }   
    }
    else if(not sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        if(cameraVelocity.x > 0){
            cameraVelocity.x -= 0.2;
        }
        else{
            cameraVelocity.x = 0;
        }
    } 
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        if(cameraVelocity.x >= -4){
            // std::cout << "lol";
            cameraVelocity.x -= 0.1;
        }   
    }
    else if(not sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        if(cameraVelocity.x < 0){
            cameraVelocity.x += 0.2;
        }
        else{
            cameraVelocity.x = 0;
        }
    }
}

struct WindowSize readWindowSize(const int argc, const char** argv){

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