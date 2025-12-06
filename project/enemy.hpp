#pragma once
#include <SFML/Graphics.hpp>


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

    sf::Vector2f target;


public:
    // Konstruktor
    Enemy(const sf::Texture& texture, float randY)
        : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyœlnego kontruktora, wiêc musimy upewniæ siê, ¿e sprite dostanie teksturê zanim bêdziemy wykonywaæ na nim jakiekolwiek operacje
    {
        sprite.setPosition({ 1280, randY });
        sprite.setScale({ 2, 2 });

        timeSinceLastFrame = 0.0f;
        frameDuration = 0.2f; // D³ugoœæ animacji w sekundach
        currentFrame = 0;
        totalFrames = 2;

        health = 1.0f;
        demage = 1.0f;
        range = 1.0f;
        accuracy = 1.0f;
        speed = 1.0f;
		walkingSpeed = 50.0f;

        target = { 10,10 };
    }

    // Funkcje

    // Renderowanie
    void update(float dt) {

        sprite.move({ -(walkingSpeed*dt), 0 });

        //WYMAGA AKTUALIZACJI SPRITE
        //timeSinceLastFrame += dt;

        //if (timeSinceLastFrame >= frameDuration)
        //{
        //    currentFrame++; // Przejœcie do nastêpnej ramki

        //    if (currentFrame >= totalFrames) currentFrame = 0; // Powrót do pierwszej ramki

        //    sprite.setTextureRect(sf::IntRect({ currentFrame * 16, 0 }, { 16, 32 })); // Kolejne klatki

        //    timeSinceLastFrame -= frameDuration;
        //}
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    // Settery
    void setTarget(sf::Vector2f newTarget) {
        target = newTarget;
    }

    // Gettery
    sf::Vector2f getPosition() {
        return sprite.getPosition();
    }
};