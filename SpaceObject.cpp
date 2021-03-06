#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "SpaceObject.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#define isNegative(x) ((x) < 0)

SpaceObject::SpaceObject(std::string name, sf::Vector2f mposition, const float factor, float radius, sf::Color color, float mass) : sf::CircleShape{radius*factor}, mass{mass}, m_to_px_factor{factor}, name{name} {
    setMPosition(mposition);
    setFillColor(color);
}

// the functions with M should be used to scale everything on the screen
void SpaceObject::moveMVel(float dt){
    moveM(mvelocity, dt);
}
    
sf::Vector2f SpaceObject::getMPosition(){
	return sf::Vector2f(getPosition().x/m_to_px_factor + getMRadius(), getPosition().y/m_to_px_factor + getMRadius());
}

void SpaceObject::setMRadius(float radius){
    setRadius(radius * m_to_px_factor);
}

float SpaceObject::getMRadius(){
    return getRadius()/m_to_px_factor;
}

void SpaceObject::setMPosition(sf::Vector2f position){
	setPosition(sf::Vector2f(position.x * m_to_px_factor - getRadius(), position.y * m_to_px_factor - getRadius()));
}

sf::Vector2f SpaceObject::operator-(SpaceObject &other){
    return sf::Vector2f(getMPosition() - other.getMPosition());
}

float SpaceObject::distance(SpaceObject& other){
    sf::Vector2f thispos = getMPosition();
    sf::Vector2f otherpos = other.getMPosition();
    return std::sqrt((otherpos.x - thispos.x) * (otherpos.x - thispos.x) + (otherpos.y - thispos.y) * (otherpos.y - thispos.y));
}

// see how much attracted by other object
float SpaceObject::Gacc(SpaceObject &other){
    return (GRAVITATIONAL_CONSTANT*other.mass)/(distance(other) * distance(other));
}

        
float SpaceObject::Gacc(float other_mass, float distance){
    return (GRAVITATIONAL_CONSTANT*other_mass)/(distance * distance);
}

void SpaceObject::Gvel(SpaceObject &other, float dt){
    float acceleration = Gacc(other);
    sf::Vector2f distvec = (*this) - other;
    sf::Vector2f velvec;


    bool x_negative = isNegative(distvec.x);
    bool y_negative = isNegative(distvec.y);

    distvec.x = abs(distvec.x);
    distvec.y = abs(distvec.y);

    velvec.x = (distvec.x / (distvec.x+distvec.y)) * acceleration;
    velvec.y = (distvec.y / (distvec.x+distvec.y)) * acceleration;

    if(x_negative != isNegative(velvec.x)){
        velvec.x = -velvec.x;
    }
    if(y_negative != isNegative(velvec.y)){
        velvec.y = -velvec.y;
    }
            
            
    velvec.x *= dt;
    velvec.y *= dt;

    velvec.x = -velvec.x;
    velvec.y = -velvec.y;

    mvelocity += velvec;
}

bool SpaceObject::window(){

    
    if(!showWindow){
        return false;
    }
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
    if(!ImGui::Begin(name.c_str(), &showWindow)){
        ImGui::End();   
        return false;
    }
        if(ImGui::BeginMenuBar()){
            ImGui::Button("close");
            ImGui::EndMenuBar();
        }
        static float mposarr[2];
        mposarr[0] = getMPosition().x;
        mposarr[1] = getMPosition().y;
        ImGui::DragFloat2("mposition", mposarr, 500);
        setMPosition(sf::Vector2f(mposarr[0], mposarr[1]));
        

        ImGui::DragFloat("mass", &mass, 10000);
        static float mvelarr[2];
        mvelarr[0] = mvelocity.x;
        mvelarr[1] = mvelocity.y;
        ImGui::DragFloat2("velocity", mvelarr);
        mvelocity.x = mvelarr[0];
        mvelocity.y = mvelarr[1];

        if(ImGui::Button("close")){
            showWindow = false;
        }
        
    ImGui::End();   
    return true;
}

bool SpaceObject::ispressed(const sf::RenderWindow& window, const sf::View& view){
    ImGuiIO& io = ImGui::GetIO();
    const float pxradius = getRadius();
    if(io.WantCaptureMouse){
        return false;
    }

    sf::Vector2f pxpos = getPosition();
    pxpos.x += pxradius; 
    pxpos.y += pxradius; 
    std::cout << "previous: " << pxpos.x << ", " << pxpos.y << '\n';
    pxpos.x += window.getSize().x / 2 - view.getCenter().x ; 
    pxpos.y += window.getSize().y / 2 - view.getCenter().y ; 
    std::cout << "now: " << pxpos.x << ", " << pxpos.y << '\n';

    sf::Vector2i mousepos = sf::Mouse::getPosition(window);

    pxpos.x = pxpos.x - mousepos.x;
    pxpos.y = pxpos.y - mousepos.y;

    bool pressednow = false;
    static bool pressedprevious = false;

    if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        pressedprevious = false;
        return false;
    }
    if(std::sqrt(pxpos.x*pxpos.x+pxpos.y*pxpos.y) < pxradius){ // Pythagoras
        pressednow = true;
        if(pressedprevious){
            pressednow = false;
        }
        pressedprevious = true;
    }
    return pressednow;
}

bool SpaceObject::ismouseon(const sf::RenderWindow& window, const sf::View& view){
    const float pxradius = getRadius();
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse){
        return false;
    }

    sf::Vector2f pxpos = getPosition();
    pxpos.x += pxradius; 
    pxpos.y += pxradius; 
    sf::Vector2i mousepos = sf::Mouse::getPosition(window);

    pxpos.x = pxpos.x - mousepos.x;
    pxpos.y = pxpos.y - mousepos.y;

    if(std::sqrt(pxpos.x*pxpos.x+pxpos.y*pxpos.y) < pxradius){ // Pythagoras
        return true;
    }

    return false;
}

void SpaceObject::moveM(sf::Vector2f vector, float dt){
    move(sf::Vector2f(vector.x * m_to_px_factor * dt, vector.y * m_to_px_factor * dt));
}

bool SpaceObject::isme(SpaceObject& other){
    return &other == this;
}

std::string SpaceObject::getName(){
    return name;
}

std::ostream& operator<<(std::ostream& os, SpaceObject& obj){
    os << "SpaceObject(name = " << obj.getName() << ", " << "mposition = (" << obj.getMPosition().x << ", " << obj.getMPosition().y << "), position = (" << obj.getPosition().x << ", " << obj.getPosition().y << "), mvelocity = (" << obj.mvelocity.x << ", " << obj.mvelocity.y << "), mass = " << obj.mass << ")"; 
    return os;
}