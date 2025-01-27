#include <SFML/Graphics.hpp>
#include "vector2.cpp"

class Player {
public:
	sf::Sprite sprite;
	sf::Texture texture;

	int health = 10;
	float speed = 8.f;
	float weight = 2.5f;

	Vector2 force;
	Vector2 position;

	Player() = default;

	Player(int h, Vector2 pos) {
		health = h;
		position = pos;
		force = Vector2{ 0.f, 0.f };
	}
};