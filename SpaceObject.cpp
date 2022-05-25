#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "SpaceObject.hpp"


SpaceObject::SpaceObject(sf::Vector2f mposition, const float factor, float radius, sf::Color color, float mass) : sf::CircleShape{radius*factor}, mass{mass}, m_to_px_factor{factor} {
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



void SpaceObject::moveM(sf::Vector2f vector, float dt){
    move(sf::Vector2f(vector.x * m_to_px_factor * dt, vector.y * m_to_px_factor * dt));
}

std::ostream& operator<<(std::ostream& os, SpaceObject& obj){
    os << "SpaceObject(mposition = (" << obj.getMPosition().x << ", " << obj.getMPosition().y << "), position = (" << obj.getPosition().x << ", " << obj.getPosition().y << "), mvelocity = (" << obj.mvelocity.x << ", " << obj.mvelocity.y << ") , mass = " << obj.mass << ")"; 
    return os;
}