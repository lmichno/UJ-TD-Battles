#include "shooter.hpp"
#include <iostream>
#include "enemy.hpp"
#include "bullet.hpp"
#include <cmath>
#include <random>

// Forward declaration dla funkcji losowych
extern std::mt19937 rng;
float randFloat(float a, float b);

// Konstruktor
Shooter::Shooter(const sf::Texture& texture, float randX, float randY)
    : sprite(texture) // sf::Sprite dla SFML 3.0> nie ma domyślnego kontruktora, więc musimy upewnić się, że sprite dostanie teksturę zanim będziemy wykonywać na nim jakiekolwiek operacje
{
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 16, 32 }));

    sprite.setPosition({ randX, randY });
    sprite.setScale({ 1.5f, 1.5f });
    
    timeSinceLastFrame = 0.0f;
    frameDuration = 0.2f; // Długość animacji w sekundach
    currentFrame = 0;
    totalFrames = 2;

    health = 3.0f;
    demage = 1.0f;
    range = 700.0f;

    target = { 0,0 };

    shootCooldown = 2.0f; // Czas między strzałami
    shootCooldownTimer = 0.0f;
    shootCooldownVariation = randFloat(0.5f, 1.5f); // Losowy offset: 0.5x do 1.5x normalnego cooldownu
}

// Funkcje

void Shooter::notifyEnemies() {
    for (Enemy* enemy : enemies) {
        if (enemy) enemy->setTarget(nullptr); // Ustawienie celu na nullptr gdy shooter jest martwy
    }

    enemies.clear(); // Wyczyszczenie listy wrogów
}

void Shooter::shoot(Enemy* target, const sf::Texture& bulletTexture)
{
    if (target == nullptr || shootCooldownTimer > 0.0f)
        return;

    sf::Vector2f shooterPos = getPosition();
    sf::Vector2f targetPos = target->getPosition();
    
    // Oblicz dystans do celu
    float distance = std::sqrt(
        std::pow(shooterPos.x - targetPos.x, 2) +
        std::pow(shooterPos.y - targetPos.y, 2)
    );

    // Strzelaj tylko gdy cel jest w zasięgu
    if (distance <= range)
    {
        bullets.push_back(std::make_unique<Bullet>(
            bulletTexture,
            shooterPos,
            target,
            1500.0f,  // prędkość pocisku
            demage
        ));
        shootCooldownTimer = shootCooldown * shootCooldownVariation; // Losowy offset
    }
}
// czyszczenie wskaznikow
void Shooter::cleanupEnemies()
{
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](Enemy* e) {
                return e == nullptr || !e->isAlive();
            }),
        enemies.end());
}





Enemy* Shooter::findNearestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies)
{
    Enemy* nearest = nullptr;
    float nearestDistance = range;

    sf::Vector2f shooterPos = getPosition();

    for (auto& enemy : enemies)
    {
        if (!enemy->isAlive())
            continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = std::sqrt(
            std::pow(shooterPos.x - enemyPos.x, 2) +
            std::pow(shooterPos.y - enemyPos.y, 2)
        );

        if (distance < nearestDistance)
        {
            nearestDistance = distance;
            nearest = enemy.get();
        }
    }

    return nearest;
}

void Shooter::updateBullets(float dt, std::vector<std::unique_ptr<Enemy>>& enemies)
{
    shootCooldownTimer -= dt;

    // Aktualizuj wszystkie pociski
    for (auto& bullet : bullets)
    {
        bullet->update(dt);

        // Sprawdź czy pocisk trafił cel
        if (bullet->hasReachedTarget())
        {
            Enemy* target = bullet->getTarget();
            if (target != nullptr)
            {
                target->takeDamage(bullet->getDamage());
            }
        }
    }

    // Usuń martwe pociski
    auto it = std::remove_if(bullets.begin(), bullets.end(),
        [](const std::unique_ptr<Bullet>& b) {
            return !b->isAlive() || b->hasReachedTarget();
        });
    bullets.erase(it, bullets.end());
}

void Shooter::drawBullets(sf::RenderWindow& window)
{
    for (auto& bullet : bullets)
    {
        bullet->draw(window);
    }
}

// Renderowanie
void Shooter::update(float dt) {
    timeSinceLastFrame += dt;

    if (timeSinceLastFrame >= frameDuration)
    {
        currentFrame++; // Przejście do następnej ramki

        if (currentFrame >= totalFrames) currentFrame = 0; // Powrót do pierwszej ramki

        sprite.setTextureRect(sf::IntRect({ currentFrame * 32, 0 }, { 32, 64 })); // Kolejne klatki

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
