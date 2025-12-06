#include "enemy.hpp"
#include "shooter.hpp"
#include <iostream>

// Konstruktor
Enemy::Enemy(const sf::Texture& texture, float randY, Shooter* shooter)
    : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyœlnego kontruktora, wiêc musimy upewniæ siê, ¿e sprite dostanie teksturê zanim bêdziemy wykonywaæ na nim jakiekolwiek operacje
{
    sprite.setPosition({ 1280, randY });
    sprite.setScale({ -2, 2 });

    timeSinceLastFrame = 0.0f;
    frameDuration = 0.2f; // D³ugoœæ animacji w sekundach
    currentFrame = 0;
    totalFrames = 2;

    health = 1.0f;
    demage = 1.0f;
    range = 400.0f;
    accuracy = 1.0f;
    speed = 1.0f;
    walkingSpeed = 50.0f;

    target = shooter;
    if (shooter) targetPos = shooter->getPosition();
}

//Funkcje
void Enemy::update(float dt) {

    if (sprite.getPosition().x - targetPos.x > range)
    {
        sprite.move({ -(walkingSpeed * dt), 0 });

        timeSinceLastFrame += dt;

        if (timeSinceLastFrame >= frameDuration)
        {
            currentFrame++; // Przejœcie do nastêpnej ramki

            if (currentFrame >= totalFrames) currentFrame = 0; // Powrót do pierwszej ramki

            sprite.setTextureRect(sf::IntRect({ currentFrame * 16, 0 }, { 16, 32 })); // Kolejne klatki

            timeSinceLastFrame -= frameDuration;
        }
    }
    else {
        if (currentFrame == 0)
        {
            // frame 0 w czasie speed
            timeSinceLastFrame += dt;

            
            sprite.setTextureRect(sf::IntRect({ 0, 32 }, { 16, 32 }));

            if (timeSinceLastFrame >= speed)
            {
                // wchodzimy w jednorazow¹ klatkê ataku - frame 1
                currentFrame = 1;
                timeSinceLastFrame = 0.0f;

                sprite.setTextureRect(sf::IntRect({ currentFrame * 16, 32 }, { 16, 32 }));
            }
        }
        else if (currentFrame == 1)
        {
            timeSinceLastFrame += dt;

            sprite.setTextureRect(sf::IntRect({ currentFrame * 16, 32 }, { 16, 32 }));

            if (timeSinceLastFrame >= frameDuration)
            {
                currentFrame = 0;
                timeSinceLastFrame = 0.0f;

                sprite.setTextureRect(sf::IntRect({ 0, 32 }, { 16, 32 }));
            }
        }
        else
        {
            // zabezpieczenie
            currentFrame = 0;
            timeSinceLastFrame = 0.0f;
            sprite.setTextureRect(sf::IntRect({ 0, 32 }, { 16, 32 }));
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

//Settery
void Enemy::setTarget(Shooter* newTarget) {
    target = newTarget;
    if (newTarget) targetPos = newTarget->getPosition();
}

//Gettery
sf::Vector2f Enemy::getPosition() {
    return sprite.getPosition();
}