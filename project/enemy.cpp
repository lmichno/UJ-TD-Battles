#include "enemy.hpp"
#include "shooter.hpp"
#include <iostream>

float globalDificulty;
void HordMode(int dif) {
    switch (dif) {
    case 1:
        globalDificulty = 1.0f;
        break;
    case 2: 
        globalDificulty = 1.5f; 
        break;
    case 3: 
        globalDificulty = 2.0f; 
        break;
    default: 
        globalDificulty = dif; }

}

// Konstruktor
Enemy::Enemy(const sf::Texture& texture, float randY, Shooter* shooter,int type)
    : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyślnego kontruktora, więc musimy upewnić się, �e sprite dostanie teksturę zanim będziemy wykonywać na nim jakiekolwiek operacje
{
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 64 }));
    sprite.setPosition({ 1280, randY });

    timeSinceLastFrame = 0.0f;
    frameDuration = 0.2f; // Dlugosc animacji w sekundach
    currentFrame = 0;
    totalFrames = 2;
    
    shootCooldown = 1.0f; // Default
    shootCooldownTimer = 0.0f;
    readyToShoot = false;

    target = shooter;
    targetPos = shooter->getPosition();

    switch (type) {
    case 0: //pierwszy wrog
        sprite.setScale({ -1.5f, 1.5f });
        health = 2.0f * globalDificulty;
        demage = 1.0f * globalDificulty;
        range = 400.0f;
        speed = 2.0f;
        walkingSpeed = 100.0f;
        break;

    case 1:
        sprite.setScale({ -1.5f, 1.5f });
        health = 4.0f * globalDificulty;
        demage = 2.0f * globalDificulty;
        range = 300.0f;
        speed = 1.0f;
        walkingSpeed = 200.0f;
        break;
        
    case 2:
        sprite.setScale({ -1.5f, 1.5f });
        health = 5.0f * globalDificulty;
        demage = 2.0f * globalDificulty;
        range = 600.0f;
        speed = 1.5f;
        walkingSpeed = 50.0f;
        break;
        
    case 3:
        sprite.setScale({ -2.5f, 2.5f });
        health = 25.0f * globalDificulty;
        demage = 5.0f * globalDificulty;
        range = 300.0f;
        speed = 1.0f;
        walkingSpeed = 25.0f;
        break;

    }
}

Enemy::~Enemy() {
    if (target) {
        target->removeEnemy(this);
    }
}

void Enemy::update(float dt) {
    
    // Update pocisków niezależnie od stanu wroga
    updateBullets(dt);

    if (target != nullptr)
    {
        targetPos = target->getPosition();
        // Target center
        targetPos.x += 24.0f;
        targetPos.y += 48.0f;
    }
    
    if (sprite.getPosition().x - targetPos.x > range || target == nullptr || targetPos.x < 0)
    {
        sprite.move({ -(walkingSpeed * dt), 0 });

        timeSinceLastFrame += dt;

        if (timeSinceLastFrame >= frameDuration)
        {
            currentFrame++; 

            if (currentFrame >= totalFrames) currentFrame = 0; 

            sprite.setTextureRect(sf::IntRect({ currentFrame * 32, 0 }, { 32, 64 })); 

            timeSinceLastFrame -= frameDuration;
        }
    }
    else {
        shootCooldownTimer -= dt;

        if (currentFrame == 0)
        {
            timeSinceLastFrame += dt;

            sprite.setTextureRect(sf::IntRect({ 0, 64 }, { 32, 64 }));

            if (timeSinceLastFrame >= speed)
            {
                currentFrame = 1;
                timeSinceLastFrame = 0.0f;

                sprite.setTextureRect(sf::IntRect({ currentFrame * 32, 64 }, { 32, 64 }));

                // Signal to shoot
                readyToShoot = true;
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
            currentFrame = 0;
            timeSinceLastFrame = 0.0f;
            sprite.setTextureRect(sf::IntRect({ 0, 64 }, { 32, 64 }));
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Enemy::shoot(const sf::Texture& bulletTexture)
{
    if (target != nullptr)
    {
        sf::Vector2f startPos = getPosition();
        startPos.y += 32.0f; // Offset for better visual
        
        auto newBullet = std::make_unique<Bullet>(
            bulletTexture,
            startPos,
            target,
            1200.0f, // Speed
            demage   // Damage
        );
        newBullet->setColor(sf::Color::Red);
        bullets.push_back(std::move(newBullet));
    }
}

void Enemy::updateBullets(float dt)
{
    for (auto& bullet : bullets)
    {
        bullet->update(dt);

        if (bullet->hasReachedTarget())
        {
            Shooter* t = bullet->getTargetShooter();
            if (t != nullptr)
            {
                t->takeDamage(bullet->getDamage());
            }
        }
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<Bullet>& b) {
                return !b->isAlive() || b->hasReachedTarget();
            }),
        bullets.end());
}

void Enemy::drawBullets(sf::RenderWindow& window)
{
    for (auto& bullet : bullets)
    {
        bullet->draw(window);
    }
}

//Settery
void Enemy::setTarget(Shooter* newTarget) {
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