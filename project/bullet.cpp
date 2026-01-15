#include "bullet.hpp"
#include "enemy.hpp"
#include "shooter.hpp"
#include <cmath>

Bullet::Bullet(const sf::Texture& texture, sf::Vector2f startPos, Enemy* target, float bulletSpeed, float bulletDamage)
    : sprite(texture), targetEnemy(target), targetShooter(nullptr), speed(bulletSpeed), damage(bulletDamage)
{
    sprite.setPosition(startPos);
    sprite.setScale({ 0.5f, 0.5f });

    if (targetEnemy != nullptr)
    {
        sf::Vector2f targetPos = targetEnemy->getPosition();
        sf::Vector2f diff = targetPos - startPos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (distance != 0.0f)
        {
            direction = { diff.x / distance, diff.y / distance };
        }
        else
        {
            direction = { 400.0f, 500.0f };
        }

        // Obrac pocisk w kierunku celu
        float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;        
        sprite.setRotation(sf::degrees(angle));
    }
    else
    {
        direction = { 0.0f, 0.0f };
    }
}

Bullet::Bullet(const sf::Texture& texture, sf::Vector2f startPos, Shooter* target, float bulletSpeed, float bulletDamage)
    : sprite(texture), targetEnemy(nullptr), targetShooter(target), speed(bulletSpeed), damage(bulletDamage)
{
    sprite.setPosition(startPos);
    sprite.setScale({ 0.5f, 0.5f });

    if (targetShooter != nullptr)
    {
        sf::Vector2f targetPos = targetShooter->getPosition();
        // Target center of shooter
        targetPos.x += 24.0f; 
        targetPos.y += 48.0f;

        sf::Vector2f diff = targetPos - startPos;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (distance != 0.0f)
        {
            direction = { diff.x / distance, diff.y / distance };
        }
        else
        {
            direction = { -1.0f, 0.0f };
        }

        float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;
        sprite.setRotation(sf::degrees(angle));
    }
    else
    {
        direction = { 0.0f, 0.0f };
    }
}

void Bullet::update(float dt)
{
    if (isAlive())
    {
        sprite.move({ direction.x * speed * dt, direction.y * speed * dt });
    }
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Vector2f Bullet::getPosition() const
{
    return sprite.getPosition();
}

Enemy* Bullet::getTarget() const
{
    return targetEnemy;
}

Shooter* Bullet::getTargetShooter() const
{
    return targetShooter;
}

bool Bullet::hasReachedTarget() const
{
    sf::Vector2f targetPos;
    if (targetEnemy != nullptr) {
        targetPos = targetEnemy->getPosition();
    } else if (targetShooter != nullptr) {
        targetPos = targetShooter->getPosition();
        targetPos.x += 24.0f; 
        targetPos.y += 48.0f;
    } else {
        return true;
    }

    sf::Vector2f bulletPos = getPosition();
    float dx = bulletPos.x - targetPos.x;
    float dy = bulletPos.y - targetPos.y;
    float distanceSq = dx * dx + dy * dy;

    return distanceSq < (50.0f * 50.0f); // Trafienie w zasiegu 50 pikseli
}

bool Bullet::isAlive() const
{
    // Pocisk martwy jesli jest poza oknem lub bez celu
    sf::Vector2f pos = getPosition();
    bool hasTarget = (targetEnemy != nullptr || targetShooter != nullptr);
    bool inWindow = pos.x > -50 && pos.x < 1330 && pos.y > -50 && pos.y < 770;
    return inWindow && hasTarget;
}

float Bullet::getDamage() const
{
    return damage;
}

void Bullet::setColor(const sf::Color& color)
{
    sprite.setColor(color);
}
