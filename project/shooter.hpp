#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "enemy.hpp"


class Shooter
{
private:
    sf::Sprite sprite;

    float timeSinceLastFrame;
    float frameDuration;
    int currentFrame;
    int totalFrames;

    float health;
    float demage;
    float range;
    float accuracy;
    float speed;

    sf::Vector2f target;
    std::vector<Enemy*> enemies;


public:
    // Konstruktor
    Shooter(const sf::Texture& texture, float randX, float randY);

    // Funkcje

    void notifyEnemies();

    // Renderowanie
    void update(float dt);

    void draw(sf::RenderWindow& window);

    // Settery
    void setTarget(sf::Vector2f newTarget);
    void addEnemy(Enemy* enemy);
	void takeDamage(float dmg);

    // Gettery
    sf::Vector2f getPosition();
	float getHealth();
};