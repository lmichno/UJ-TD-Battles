#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

extern float globalDificulty;
void HordMode(int dif);

class Shooter;

class Enemy
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
    float walkingSpeed;

    Shooter* target = nullptr;
	sf::Vector2f targetPos;


public:
    // Konstruktor
    Enemy(const sf::Texture& texture, float randY, Shooter* shooter, int type);

    // Funkcje
    //asdsad
    // Renderowanie
    void update(float dt);

    void draw(sf::RenderWindow& window);

    // Settery
    void setTarget(Shooter* newTarget);
    void takeDamage(float dmg);

    // Gettery
    sf::Vector2f getPosition() const;
	Shooter* getTarget() const;
    float getHealth() const;
    bool isAlive() const;
};