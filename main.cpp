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

#define arrtocol(arr) sf::Color(arr[0] * 255, arr[1] * 255, arr[2] * 255, arr[3] * 255)

struct WindowSize{
    int width, height;
};
struct NewPlanet{
    float mposition[2] = {0, 0}, mvelocity[2] = {0, 0};
    float color[4] = {0, 0, 1, 1};
    float mass = 1000, radius = 6371000;
    char name[100] = {'\0'};
    char errorMsg[100] = {'\0'};
};

struct WindowSize readWindowSize(const int argc, const char** argv);
void UpdateCameraVelocity(sf::Vector2f &cameraVelocity);
SpaceObject createSpaceObject();
bool spaceobjname(std::vector<SpaceObject>& objects, const std::string& name);
void initPrototypeValues(SpaceObject& prototype, NewPlanet& obj, int width, int height);


int main(const int argc, const char** argv){

    auto [width, height] = readWindowSize(argc, argv);
    
    std::cout << "\e[?25l"; //cursor off

    sf::RenderWindow window(sf::VideoMode(width, height), "lol", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);
    if(!ImGui::SFML::Init(window)){
        std::cout << "imgui failed!" << '\n';
        return EXIT_FAILURE;
    }
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.6f;
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    

    sf::Vector2i viewpos{0, 0};
    sf::View view = window.getDefaultView();

    sf::Event event;
    sf::Clock imguiClock;
    sf::Clock deltaClock;
    float dt;
    float passedTime = 0;
    float timeFactor = 1;

    // for creating a new planet with the Gui
    NewPlanet newObj;    
    SpaceObject prototypeObj{"prototype"};
    prototypeObj.m_to_px_factor = FACTOR;
    prototypeObj.setFillColor(sf::Color::Transparent);
    bool createWindowOpened = false;

    sf::Vector2f cameraVelocity{0, 0};

    // TODO: add deleting with Gui
    std::vector<SpaceObject> spaceObjects{};

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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::N) and sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            // this code is executed once when the window is opened
            if(not createWindowOpened){
                // this code is executed once when the create window is opened
                initPrototypeValues(prototypeObj, newObj, width, height);
            }

            createWindowOpened = true;
        }


        UpdateCameraVelocity(cameraVelocity);
        view.move(cameraVelocity);


        for(SpaceObject &i: spaceObjects){
            i.moveMVel(dt);
            for(SpaceObject &j: spaceObjects){
                if(!i.isme(j)){
                    i.Gvel(j, dt);
                }
            }
        }
        
        ImGui::SFML::Update(window, imguiClock.restart());
        dt = deltaClock.restart().asSeconds();

        //imgui menu bar 
        if(ImGui::BeginMainMenuBar()){
            if(ImGui::BeginMenu("file")){
                if(ImGui::MenuItem("close window", "esc")){
                    window.close();
                }

                if(ImGui::MenuItem("create planet", "ctrl + n", &createWindowOpened)){
                    initPrototypeValues(prototypeObj, newObj, width, height);
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
        // Gui for creating a new planet
        if(createWindowOpened){
            ImGui::SetNextWindowSize(ImVec2(550, 750), ImGuiCond_FirstUseEver);
            ImGui::Begin("create planet", &createWindowOpened);
                ImGui::InputText("name", newObj.name, 100);
                ImGui::InputFloat2("mposition", newObj.mposition);
                ImGui::InputFloat2("mvelocity", newObj.mvelocity);
                ImGui::InputFloat("mass", &newObj.mass);
                ImGui::InputFloat("radius", &newObj.radius);
                ImGui::ColorPicker4("color", newObj.color);
                
                
                if(ImGui::Button("create")){
                    if(newObj.name[0] == '\0'){
                        strncpy_s(newObj.errorMsg, "must provide a name", 100);
                    }
                    else if(spaceobjname(spaceObjects, newObj.name)){
                        strncpy_s(newObj.errorMsg, "name already exists", 100);
                    }
                    else{
                        spaceObjects.push_back(SpaceObject(newObj.name, sf::Vector2f(newObj.mposition[0], newObj.mposition[1]), FACTOR, newObj.radius, arrtocol(newObj.color), newObj.mass));
                        newObj.errorMsg[0] = '\0';
                    }
                }

                // updating the prototype object
                prototypeObj.setMPosition(sf::Vector2f(newObj.mposition[0], newObj.mposition[1]));
                prototypeObj.setFillColor(arrtocol(newObj.color));
                prototypeObj.setMRadius(newObj.radius);

                ImGui::Text(newObj.errorMsg); // shows an error message if a user input is invalid
            ImGui::End();
        }

        // hides prototype object if not needed
        if(not createWindowOpened){
            prototypeObj.setFillColor(sf::Color::Transparent);
        }
        
        // std::cout << "fill Color: " << static_cast<int>(prototypeObj.getFillColor().a) << '\n';

        // std::cout << prototypeObj.getPosition().x << ", " << prototypeObj.getPosition().y << '\n';
        for(int i = 0; i < spaceObjects.size(); ++i){
            std::cout << spaceObjects[i] << i << '\n';
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

        window.draw(prototypeObj);

        // SFML screen update
        ImGui::SFML::Render(window);
        window.display();
        window.setView(view);
    }


    std::cout << "\e[?25h" << '\n'; //cursor on
    return EXIT_SUCCESS;
}


void initPrototypeValues(SpaceObject& prototype, NewPlanet& obj, int width, int height){
    // this code is executed once when the create window is opened
    obj.color[0] = 0;
    obj.color[1] = 0;
    obj.color[2] = 1;
    obj.color[3] = 1;
    obj.mass = 100;
    obj.mposition[0] = absolutePos(width/2);
    obj.mposition[1] = absolutePos(height/2);
    obj.radius = 6371000;
     
    prototype.setMPosition(sf::Vector2f(obj.mposition[0], obj.mposition[1]));
    prototype.setFillColor(arrtocol(obj.color));
    prototype.setMRadius(obj.radius);
}

void UpdateCameraVelocity(sf::Vector2f &cameraVelocity){
    
    // handling the keyboard input for the camera movement
    // TODO: add mouse dragging

    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureKeyboard){
        cameraVelocity.x = 0;
        cameraVelocity.y = 0;
        return;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        if(cameraVelocity.y <= 4){
            cameraVelocity.y += 0.2;
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
            cameraVelocity.y -= 0.2;
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
            cameraVelocity.x += 0.2;
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
            cameraVelocity.x -= 0.2;
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

bool spaceobjname(std::vector<SpaceObject>& objects, const std::string& name){
    for(SpaceObject& i: objects){
        if(name == i.getName()){
            return true;
        }
    }
    return false;
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