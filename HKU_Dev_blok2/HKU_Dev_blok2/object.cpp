#include <SFML/Graphics.hpp>
#include "vector2.cpp"

class Object {
public:
	sf::Sprite sprite;
	Vector2 position;
	Vector2 force;
	float size = 70.f;

	Object(sf::Sprite s, float p, float v) {
		sprite = s;
		position = Vector2{p, -64.f};
		force = Vector2{0.f, v};
	}
};