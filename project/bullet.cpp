#include "bullet.hpp"
#include "enemy.hpp"
#include <cmath>

Bullet::Bullet(const sf::Texture& texture, sf::Vector2f startPos, Enemy* targetEnemy, float bulletSpeed, float bulletDamage)
    : sprite(texture), target(targetEnemy), speed(bulletSpeed), damage(bulletDamage)
{
    sprite.setPosition(startPos);
    sprite.setScale({ 0.5f, 0.5f });

    if (target != nullptr)
    {
        sf::Vector2f targetPos = target->getPosition();
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

        // Obróæ pocisk w kierunku celu
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
    if (target != nullptr && isAlive())
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
    return target;
}

bool Bullet::hasReachedTarget() const
{
    if (target == nullptr)
        return true;

    sf::Vector2f bulletPos = getPosition();
    sf::Vector2f targetPos = target->getPosition();
    float distance = std::sqrt(
        std::pow(bulletPos.x - targetPos.x, 2) +
        std::pow(bulletPos.y - targetPos.y, 2)
    );

    return distance < 50.0f; // Trafienie w zasiêgu 30 pikseli
}

bool Bullet::isAlive() const
{
    // Pocisk martwy jeœli jest poza oknem lub bez celu
    sf::Vector2f pos = getPosition();
    return pos.x > 0 && pos.x < 1280 && pos.y > 0 && pos.y < 720 && target != nullptr;
}

float Bullet::getDamage() const
{
    return damage;
}
