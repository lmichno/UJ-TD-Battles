#include "enemy.hpp"
#include "shooter.hpp"
#include <iostream>

// Konstruktor
Enemy::Enemy(const sf::Texture& texture, float randY, Shooter* shooter,int type)
    : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyślnego kontruktora, więc musimy upewnić się, �e sprite dostanie teksturę zanim będziemy wykonywać na nim jakiekolwiek operacje
{
    switch (type) {
    case 0: //pierwszy wrog
        sprite.setPosition({ 1280, randY });
        sprite.setScale({ -1.5f, 1.5f });

        timeSinceLastFrame = 0.0f;
        frameDuration = 0.2f; // Długość animacji w sekundach
        currentFrame = 0;
        totalFrames = 2;

        health = 1.0f;
        demage = 1.0f;
        range = 400.0f;
        accuracy = 1.0f;
        speed = 1.0f;
        walkingSpeed = 200.0f;

        target = shooter;
        targetPos = shooter->getPosition();
        break;

    case 1:
        sprite.setPosition({ 1280, randY });
        sprite.setScale({ -1.5f, 1.5f });

        timeSinceLastFrame = 0.0f;
        frameDuration = 0.2f; // Długość animacji w sekundach
        currentFrame = 0;
        totalFrames = 2;

        health = 1.0f;
        demage = 2.0f;
        range = 500.0f;
        accuracy = 1.0f;
        speed = 1.5f;
        walkingSpeed = 100.0f;

        target = shooter;
        targetPos = shooter->getPosition();
        break;
    case 2:
        sprite.setPosition({ 1280, randY });
        sprite.setScale({ -1.5f, 1.5f });

        timeSinceLastFrame = 0.0f;
        frameDuration = 0.2f; // Długość animacji w sekundach
        currentFrame = 0;
        totalFrames = 2;

        health = 1.0f;
        demage = 2.0f;
        range = 500.0f;
        accuracy = 1.0f;
        speed = 2.0f;
        walkingSpeed = 300.0f;

        target = shooter;
        targetPos = shooter->getPosition();
        break; 
    case 3: 
          sprite.setPosition({ 1280, randY });
          sprite.setScale({ -2.5f, 2.5f });

          timeSinceLastFrame = 0.0f;
          frameDuration = 0.2f; // Długość animacji w sekundach
          currentFrame = 0;
          totalFrames = 2;

          health = 1.0f;
          demage = 2.5f;
          range = 600.0f;
          accuracy = 1.0f;
          speed = 2.0f;
          walkingSpeed = 200.0f;

          target = shooter;
          targetPos = shooter->getPosition();
          break;
    }
}

//Funkcje
void Enemy::update(float dt) {

    if (sprite.getPosition().x - targetPos.x > range || target == nullptr || targetPos.x < 0)
    {
        sprite.move({ -(walkingSpeed * dt), 0 });

        timeSinceLastFrame += dt;

        if (timeSinceLastFrame >= frameDuration)
        {
            currentFrame++; // Przejście do następnej ramki

            if (currentFrame >= totalFrames) currentFrame = 0; // Powrót do pierwszej ramki

            sprite.setTextureRect(sf::IntRect({ currentFrame * 32, 0 }, { 32, 64 })); // Kolejne klatki

            timeSinceLastFrame -= frameDuration;
        }
    }
    else {
        if (currentFrame == 0)
        {
            // frame 0 w czasie speed
            timeSinceLastFrame += dt;


            sprite.setTextureRect(sf::IntRect({ 0, 64 }, { 32, 64 }));

            if (timeSinceLastFrame >= speed)
            {
                // wchodzimy w jednorazową klatkę ataku - frame 1
                currentFrame = 1;
                timeSinceLastFrame = 0.0f;

                sprite.setTextureRect(sf::IntRect({ currentFrame * 32, 64 }, { 32, 64 }));

                if (target != nullptr) target->takeDamage(demage);
				else targetPos = { -10000.0f, -100.0f };

            }
        }
        else if (currentFrame == 1)
        {
            timeSinceLastFrame += dt;

            sprite.setTextureRect(sf::IntRect({ currentFrame * 32, 64 }, { 32, 64 }));

            if (timeSinceLastFrame >= frameDuration)
            {
                currentFrame = 0;
                timeSinceLastFrame = 0.0f;

                sprite.setTextureRect(sf::IntRect({ 0, 64 }, { 32, 64 }));
            }
        }
        else
        {
            // zabezpieczenie
            currentFrame = 0;
            timeSinceLastFrame = 0.0f;
            sprite.setTextureRect(sf::IntRect({ 0, 64 }, { 32, 64 }));
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

//Settery
void Enemy::setTarget(Shooter* newTarget) {
    target = newTarget;
    if (newTarget != nullptr)targetPos = newTarget->getPosition();
    else targetPos = { -10000.0f, -100.0f };
}

//Gettery
sf::Vector2f Enemy::getPosition() {
    return sprite.getPosition();
}
Shooter* Enemy::getTarget() {
    return target;
}