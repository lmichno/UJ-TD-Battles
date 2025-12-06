#include "shooter.hpp"
#include <iostream>
#include "enemy.hpp"

// Konstruktor
Shooter::Shooter(const sf::Texture& texture, float randX, float randY)
    : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyœlnego kontruktora, wiêc musimy upewniæ siê, ¿e sprite dostanie teksturê zanim bêdziemy wykonywaæ na nim jakiekolwiek operacje
{
    sprite.setPosition({ randX, randY });
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

    target = { 0,0 };
}

// Funkcje

// Renderowanie
void Shooter::update(float dt) {
    timeSinceLastFrame += dt;

    if (timeSinceLastFrame >= frameDuration)
    {
        currentFrame++; // Przejœcie do nastêpnej ramki

        if (currentFrame >= totalFrames) currentFrame = 0; // Powrót do pierwszej ramki

        sprite.setTextureRect(sf::IntRect({ currentFrame * 16, 0 }, { 16, 32 })); // Kolejne klatki

        timeSinceLastFrame -= frameDuration;
    }
}

void Shooter::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Settery
void Shooter::setTarget(sf::Vector2f newTarget) {
    target = newTarget;
}
void Shooter::addEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

// Gettery
sf::Vector2f Shooter::getPosition() {
    return sprite.getPosition();
}