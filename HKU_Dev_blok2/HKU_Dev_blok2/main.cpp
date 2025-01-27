#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <random>
#include "player.cpp"
#include "object.cpp"

void startup();
void update(sf::RenderWindow& window, float& dt, sf::Clock& stripeTimer, sf::Clock& enemyTimer);
void manageStripes(sf::RenderWindow& window, sf::Clock& stripeTimer, float& dt);
void manageEnemies(sf::RenderWindow& window, sf::Clock& enemyTimer, float& dt);

bool isAlive = true;
float deathTimer = 5.f;

float stripeSpawnTimer = 0.15f;
float enemySpawnTimer = 2.0f;

sf::Font font;

sf::Text deathText;
sf::Text Score;
int score = 0;

Player player;
std::deque<Object> stripes;
std::deque<Object> enemies;

sf::Sprite backgroundSprite;
sf::Texture backgroundTexture;

sf::Texture stripeTexture;
sf::Texture enemyTexture;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "Speed Racer Plus");
    sf::Clock clock;
    sf::Clock stripeTimer;
    sf::Clock enemyTimer;

    startup();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(backgroundSprite);

        float dt = clock.restart().asSeconds();
        update(window, dt, stripeTimer, enemyTimer);

        if (enemySpawnTimer > 0.2) {
            enemySpawnTimer -= 0.03 * dt;
        }

        if (!isAlive) {
            window.draw(deathText);

            if (deathTimer > 0.f) {
                deathTimer -= dt;
            }
            else {
                //quit program
                window.close();
            }
        }

        window.display();
    }

    return 0;
}

void startup() {
    std::cout << "starting up...";

    //loading textures
    stripeTexture.loadFromFile("stripe.png");
    enemyTexture.loadFromFile("white_car.png");

    //setting up background
    if (!backgroundTexture.loadFromFile("background.png")) {
        // error...
        std::cout << "error loading background texture";
    }

    sf::Sprite bgSprite;
    bgSprite.setTexture(backgroundTexture);
    backgroundSprite = bgSprite;

    //setting up player
    player = Player{ 10, Vector2{512.f, 1024.f - 300.f} };

    if (!player.texture.loadFromFile("red_car.png"))
    {
        // error...
        std::cout << "error loading player texture";
    }
    player.texture.setSmooth(false);

    sf::Sprite sprite;
    sprite.setTexture(player.texture);
    sprite.setScale(4.f, 4.f);

    player.sprite = sprite;

    if (!font.loadFromFile("default.ttf")) {
        std::cout << "error loading font";
    }

    deathText.setFont(font);
    deathText.setString("You Crashed!");
    deathText.setCharacterSize(40);
    deathText.setFillColor(sf::Color::Black);
    deathText.setPosition(256, 512);

    Score.setFont(font);
    Score.setString("Score: 0");
    Score.setCharacterSize(24);
    Score.setFillColor(sf::Color::Black);
    Score.setPosition(64, 64);
}

void update(sf::RenderWindow& window, float& dt, sf::Clock& stripeTimer, sf::Clock& enemyTimer) {
    //handle input
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    if (left && !right) {
        player.force = player.force + (Vector2{ -1, 0 } * player.speed);
    }
    else if (!left && right) {
        player.force = player.force + (Vector2{ 1, 0 } *player.speed);
    }

    if (isAlive) {
        //apply player forces
        player.position = player.position + ((player.force / player.weight) * dt);
    }

    if (player.position.x > 1024.f - 64.f || player.position.x < 0.f) {
        isAlive = false;
    }


    //apply player friction
    player.force = player.force * (0.997f * (1.f - dt));
    if (player.force.mag() < 0.1) {
        player.force = Vector2{0.f, 0.f};
    }

    manageStripes(window, stripeTimer, dt);

    manageEnemies(window, enemyTimer, dt);

    //draw player
    player.sprite.setPosition(player.position.x, player.position.y);
    window.draw(player.sprite);

    Score.setString("Score: " + std::to_string(score));
    window.draw(Score);
}

void manageStripes(sf::RenderWindow& window, sf::Clock& stripeTimer, float& dt) {
    //spawn stripes
    if (stripeTimer.getElapsedTime().asSeconds() > stripeSpawnTimer && isAlive) {
        sf::Sprite sprite;
        sprite.setTexture(stripeTexture);
        sprite.setScale(0.5f, 0.5f);

        for (int i = 0; i < 4; i++) {
            stripes.push_back(Object{ sprite, ((i + 1.f) * 185.f) + 45.f, 0.f });
        }

        stripeTimer.restart();
    }

    //draw stripes
    int pop = 0;
    for (int i = 0; i < stripes.size(); i++ ) {
        Object& obj = stripes.at(i);

        if (isAlive) {
            obj.position.y += 1100.0f * dt;
            obj.sprite.setPosition(obj.position.x, obj.position.y);
        }

        if (obj.position.y > 1024.f) {
            pop += 1;
        }
        else {
            window.draw(obj.sprite);
        }
    }

    //delete stripes
    for (int i = 0; i < pop; i++) {
        stripes.pop_front();
    }
}

void manageEnemies(sf::RenderWindow& window, sf::Clock& enemyTimer, float& dt) {
    if (!isAlive) { return; }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(128, 1024 - 256);
    std::uniform_int_distribution<> distrib2(500, 1500);

    //spawn enemies
    if (enemyTimer.getElapsedTime().asSeconds() > enemySpawnTimer && isAlive) {
        sf::Sprite sprite;
        sprite.setTexture(enemyTexture);
        sprite.setScale(4.f, 4.f);

        enemies.push_back(Object{ sprite, static_cast<float>(distrib(gen)), static_cast<float>(distrib2(gen)) });

        enemyTimer.restart();
    }

    //work enemies
    int pop = 0;
    for (int i = 0; i < enemies.size(); i++) {
        Object& obj = enemies.at(i);

        if (isAlive) {
            obj.position = obj.position + (obj.force * dt);
            obj.sprite.setPosition(obj.position.x, obj.position.y);

            if (obj.position.dist(player.position) < obj.size) {
                std::cout << "hit!";
                isAlive = false;
            }
        }

        if (obj.position.y > 1024.f) {
            pop += 1;
            score += 1;
        }
        else {
            window.draw(obj.sprite);
        }
    }

    //delete enemies
    for (int i = 0; i < pop; i++) {
        enemies.pop_front();
    }
}