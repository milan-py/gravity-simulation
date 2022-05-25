#ifndef SPACEOBJECT_HPP

#define SPACEOBJECT_HPP

constexpr float GRAVITATIONAL_CONSTANT = 6.67408e-11;
#define isNegative(x) ((x) < 0)

class SpaceObject : public sf::CircleShape{
    public: 
        SpaceObject(sf::Vector2f mposition = sf::Vector2f(0.0f, 0.0f), const float factor = 1, float radius = 50.0f, sf::Color color = sf::Color::White, float mass = 100);

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

		friend std::ostream& operator<<(std::ostream& os, const SpaceObject& obj);
        
        float mass;
        sf::Vector2f mvelocity{0, 0};
		const float m_to_px_factor;

	private:
        void moveM(sf::Vector2f vector, float dt = 1);
};

std::ostream& operator<<(std::ostream& os, SpaceObject& obj);

#endif