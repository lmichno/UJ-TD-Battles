#pragma once
#include <SFML/Graphics.hpp>

class Enemy;
class Shooter;

class Bullet
{
private:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float damage;
    Enemy* targetEnemy;
    Shooter* targetShooter;

public:
    Bullet(const sf::Texture& texture, sf::Vector2f startPos, Enemy* targetEnemy, float bulletSpeed, float bulletDamage);
    Bullet(const sf::Texture& texture, sf::Vector2f startPos, Shooter* targetShooter, float bulletSpeed, float bulletDamage);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    Enemy* getTarget() const;
    Shooter* getTargetShooter() const;
    bool hasReachedTarget() const;
    bool isAlive() const;

    float getDamage() const;
    void setColor(const sf::Color& color);
};
