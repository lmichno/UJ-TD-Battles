#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "enemy.hpp"

// Forward declaration
class Bullet;

class Shooter
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

    sf::Vector2f target;
    std::vector<Enemy*> enemies;
    std::vector<std::unique_ptr<Bullet>> bullets;
    
    float shootCooldown;
    float shootCooldownTimer;
    float shootCooldownVariation; // Losowa zmiana cooldownu

    // Animation specific
    bool isShooting = false;
    Enemy* targetForShot = nullptr;
    const sf::Texture* bulletTextureRef = nullptr;

public:
    // Konstruktor
    Shooter(const sf::Texture& texture, float randX, float randY, int type);
    ~Shooter();

    // Funkcje
    void removeEnemy(Enemy* enemy);
    void cleanupEnemies();

    void notifyEnemies();
    void shoot(Enemy* target, const sf::Texture& bulletTexture);
    void updateBullets(float dt, std::vector<std::unique_ptr<Enemy>>& enemies);
    void drawBullets(sf::RenderWindow& window);
    Enemy* findNearestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies);

    // Renderowanie
    void update(float dt);

    void draw(sf::RenderWindow& window);

    // Settery
    void setTarget(sf::Vector2f newTarget);
    void addEnemy(Enemy* enemy);
	void takeDamage(float dmg);

    // Gettery
    sf::Vector2f getPosition();
	float getHealth();
};