#include "shooter.hpp"
#include <iostream>
#include "enemy.hpp"

// Konstruktor
Shooter::Shooter(const sf::Texture& texture, float randX, float randY)
    : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyślnego kontruktora, więc musimy upewnić się, że sprite dostanie teksturę zanim będziemy wykonywać na nim jakiekolwiek operacje
{
    sprite.setPosition({ randX, randY });
    sprite.setScale({ 2, 2 });
    
    timeSinceLastFrame = 0.0f;
    frameDuration = 0.2f; // Długość animacji w sekundach
    currentFrame = 0;
    totalFrames = 2;

    health = 3.0f;
    demage = 1.0f;
    range = 1.0f;
    accuracy = 1.0f;
    speed = 1.0f;

    target = { 0,0 };
}

// Funkcje

void Shooter::notifyEnemies() {
    for (Enemy* enemy : enemies) {
        if (enemy) enemy->setTarget(nullptr); // Ustawienie celu na nullptr gdy shooter jest martwy
    }

enemies.clear(); // Wyczyszczenie listy wrogów
}

// Renderowanie
void Shooter::update(float dt) {
    timeSinceLastFrame += dt;

    if (timeSinceLastFrame >= frameDuration)
    {
        currentFrame++; // Przejście do następnej ramki

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
void Shooter::takeDamage(float dmg) {
    // Sprawdzenie czy shooter już nie żyje
    if (health <= 0) return;

    health -= dmg;
    std::cout << "Shooter took " << dmg << " damage, health now: " << health << std::endl;
    
    // Zabezpieczenie przed ujemnym życiem i wielokrotnym notify
    if (health <= 0) {
        health = 0;
        Shooter::notifyEnemies();
    }
}

// Gettery
sf::Vector2f Shooter::getPosition() {
    return sprite.getPosition();
}
float Shooter::getHealth() {
    return health;
}
