#include "enemy.hpp"
#include "shooter.hpp"
#include <iostream>

// Konstruktor
Enemy::Enemy(const sf::Texture& texture, float randY, Shooter* shooter,int type)
    : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyślnego kontruktora, więc musimy upewnić się, �e sprite dostanie teksturę zanim będziemy wykonywać na nim jakiekolwiek operacje
{
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 16, 32 }));
    sprite.setPosition({ 1280, randY });
    sprite.setScale({ -2, 2 });

    timeSinceLastFrame = 0.0f;
    frameDuration = 0.2f; // Długość animacji w sekundach
    currentFrame = 0;
    totalFrames = 2;

    target = shooter;
    targetPos = shooter->getPosition();

    switch (type) {
    case 0: //pierwszy wrog
        health = 2.0f;
        demage = 1.0f;
        range = 400.0f;
        speed = 1.0f;
        walkingSpeed = 100.0f;
        break;

    case 1:
        health = 4.0f;
        demage = 2.0f;
        range = 300.0f;
        speed = 2.0f;
        walkingSpeed = 200.0f;
        break;
    case 2:

        health = 5.0f;
        demage = 2.0f;
        range = 600.0f;
        speed = 1.5f;
        walkingSpeed = 50.0f;
        break;

    }
}

//Funkcje
void Enemy::update(float dt) {

    if (target != nullptr)
        targetPos = target->getPosition();
    
    if (sprite.getPosition().x - targetPos.x > range || target == nullptr || targetPos.x < 0)
    {
        sprite.move({ -(walkingSpeed * dt), 0 });

        timeSinceLastFrame += dt;

        if (timeSinceLastFrame >= frameDuration)
        {
            currentFrame++; // Przejście do następnej ramki

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
                // wchodzimy w jednorazową klatkę ataku - frame 1
                currentFrame = 1;
                timeSinceLastFrame = 0.0f;

                sprite.setTextureRect(sf::IntRect({ currentFrame * 16, 32 }, { 16, 32 }));

                if (target != nullptr) target->takeDamage(demage);
				else targetPos = { -10000.0f, -100.0f };

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
    if (target != nullptr && target != newTarget) {
        target->removeEnemy(this);
    }

    target = newTarget;
    if (newTarget != nullptr) targetPos = newTarget->getPosition();
    else targetPos = { -10000.0f, -100.0f };
}

void Enemy::takeDamage(float dmg)
{
    if (health <= 0) return;

    health -= dmg;
    
    if (health < 0)
    {
        health = 0;
    }
}

//Gettery
sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}
Shooter* Enemy::getTarget() const {
    return target;
}
float Enemy::getHealth() const {
    return health;
}
bool Enemy::isAlive() const {
    return health > 0;
}