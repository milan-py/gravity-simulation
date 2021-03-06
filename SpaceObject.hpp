#ifndef SPACEOBJECT_HPP
#define SPACEOBJECT_HPP

#include <imgui.h>

constexpr float GRAVITATIONAL_CONSTANT = 6.67408e-11;

class SpaceObject : public sf::CircleShape{
    public: 
        SpaceObject(std::string name, sf::Vector2f mposition = sf::Vector2f(0.0f, 0.0f), const float factor = 1, float radius = 50.0f, sf::Color color = sf::Color::White, float mass = 100);

        // the functions with M should be used to scale everything on the screen
        void moveMVel(float dt = 1);

        
		sf::Vector2f getMPosition();

        void setMRadius(float radius);

        float getMRadius();

		void setMPosition(sf::Vector2f position);

        sf::Vector2f operator-(SpaceObject &other);

        float distance(SpaceObject& other);

        // see how much attracted by other object
        float Gacc(SpaceObject &other);
        float Gacc(float other_mass, float distance);

        void Gvel(SpaceObject &other, float dt = 1);

        std::string getName();
		friend std::ostream& operator<<(std::ostream& os, const SpaceObject& obj);

        bool window(); // shows an imgui window if showWindow is true returns thing from imgui

        bool ispressed(const sf::RenderWindow& window, const sf::View& view); // checks if mouse pressed object
        bool ismouseon(const sf::RenderWindow& window, const sf::View& view); // checks if mouse is over object on and left key is pressed
        bool isme(SpaceObject& other);
        
        float mass;
        sf::Vector2f mvelocity{0, 0};
		float m_to_px_factor;
        bool showWindow = false;
        

	private:
        void moveM(sf::Vector2f vector, float dt = 1);
        const std::string name;
        ImFont* font;
        
};

std::ostream& operator<<(std::ostream& os, SpaceObject& obj);

#endif