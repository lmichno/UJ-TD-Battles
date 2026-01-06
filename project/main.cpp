#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <memory>
#include <algorithm>

#include "shooter.hpp"
#include "enemy.hpp"
#include "button.hpp"

std::mt19937 rng(std::random_device{}()); // Globalny generator losowy

float randFloat(float a, float b);
int randInt(int a, int b);

int main()
{
    // Główne operacje
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "UJ TD 1"); // Utworzenie okna

    sf::Clock clock; // Zegar do deltaTime

    window.setFramerateLimit(60); // Ustawienie 60fps

    // Zmienne
    // Używamy unique_ptr, aby adresy w pamięci były stałe (dla Enemy::target i Shooter::enemies)
    std::vector<std::unique_ptr<Shooter>> shooters;
    std::vector<std::unique_ptr<Enemy>> enemies; //
    int money = 100;
    int moneyAdd = 15;

    int currentWave = 3;

    //Pierwsza fala wrogów
    float wave1Duration = 45.0f;      // czas trwania fali 1
    float lastEnemySpawnTime1 = -5.0f; // żeby pierwszy wróg był od razu

    //Druga fala wrogów
    float wave2Duration = 45.0f; //czas trwania fali s
    float lastEnemySpawnTime2 = -3.0f; //żeby pierwszy wróg byl odrazu

    //Trzecia fala wrogów
    float wave3Duration = 45.0f; // czas trwania fali 3
    float lastEnemySpawnTime3 = -2.0f; // żeby pierwszy wróg był od razu

    sf::Clock waveClock; //czas trwania fali


    //Fonty
    sf::Font mediumGothic("ScienceGothic-Medium.ttf");

    //BOCZNY PANEL--------------------------
    // Przyciski
    Button jaguar1Button(12, 115, 100, 125, 15); // x, y, width, height
    Button jaguar2Button(12, 243, 100, 125, 30);
    Button jaguar3Button(12, 371, 100, 125, 60);
    Button jaguar4Button(12, 504, 100, 125, 120);

    // Teksty
    sf::Text text(mediumGothic);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);

    sf::Text kasa = text;
    std::string moneyStr = std::to_string(money);
    kasa.setString(moneyStr);
    kasa.setFillColor(sf::Color::Yellow);
    kasa.setOutlineThickness(1);
    kasa.setOutlineColor(sf::Color::Black);
    kasa.setPosition({ 20, 38 });

    sf::Text zarobki = text;
    std::string  moneyAddStr = std::to_string(moneyAdd) + " $/s";
    zarobki.setString(moneyAddStr);
    zarobki.setPosition({ 20, 85 });
    zarobki.setFillColor(sf::Color::Green);
    zarobki.setOutlineThickness(1);
    zarobki.setOutlineColor(sf::Color::Black);

    sf::Text jaguar1Cost = text;
    jaguar1Cost.setCharacterSize(12);
    jaguar1Cost.setFillColor(sf::Color::Yellow);
    jaguar1Cost.setOutlineThickness(0.8f);

    sf::Text jaguar2Cost = jaguar1Cost;
    sf::Text jaguar3Cost = jaguar1Cost;
    sf::Text jaguar4Cost = jaguar1Cost;

    jaguar1Cost.setString("15");
    jaguar1Cost.setPosition({ 15, 150 });
    jaguar2Cost.setString("30");
    jaguar2Cost.setPosition({ 15, 280 });
    jaguar3Cost.setString("60");
    jaguar3Cost.setPosition({ 15, 410 });
    jaguar4Cost.setString("120");
    jaguar4Cost.setPosition({ 15, 540 });
    //BOCZNY PANEL--------------------------

    // Tekstury
    sf::Texture jaguar1("jaguar1.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
    sf::Texture ludzik("jaguar1.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
    sf::Texture ludzik2("wrog2.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
    sf::Texture ludzik3("wrog3.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
    sf::Texture sidePanel("sidePanel.png");

    //Sprite
    sf::Sprite sidePanelSprite(sidePanel);


    // Operacje wstępne
    for (int i = 0; i < 20; i++)
    {
        shooters.push_back(std::make_unique<Shooter>(jaguar1, randFloat(130.f, 170.f), randFloat(0.f, 656.0f)));
    }
    /*for (int i = 0; i < 2; i++)
    {
        int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
        enemies.push_back(std::make_unique<Enemy>(ludzik, randFloat(0.f, 656.0f), shooters[randomT].get(),0));
        shooters[randomT]->addEnemy(enemies.back().get());
    }*/


    //TEMP
    sf::RectangleShape rectangle({ 5, 720 });
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition({ 190, 0 });
    sf::RectangleShape rect2({ 2, 720 });
    rect2.setFillColor(sf::Color::Black);
    rect2.setPosition({ 126, 0 });

    //Start fali
    waveClock.restart();



    while (window.isOpen()) // Główna pętla aplikacji
    {
        while (const std::optional event = window.pollEvent()) // Sprawdzanie eventów
        {
            if (event->is<sf::Event::Closed>()) // Obsługa zamknięcia okna
                window.close();

            // Obsługa kliknięć myszy
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                jaguar1Button.onMousePressed();
                jaguar2Button.onMousePressed();
                jaguar3Button.onMousePressed();
                jaguar4Button.onMousePressed();
            }

            if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                jaguar1Button.onMouseReleased();
                jaguar2Button.onMouseReleased();
                jaguar3Button.onMouseReleased();
                jaguar4Button.onMouseReleased();
            }
        }

        // Wyliczanie dt (czas między kolejnymi klatkami)
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        float waveTime = waveClock.getElapsedTime().asSeconds();

        //FALA 1 
        if (currentWave == 1)
        {
            if (waveTime < wave1Duration)
            {
                float currentSpawnInterval;

                if (waveTime < 15.0f)
                    currentSpawnInterval = 5.0f;
                else if (waveTime < 20.0f)
                    currentSpawnInterval = 3.0f;
                else if (waveTime < 30.0f)
                    currentSpawnInterval = 2.0f;
                else
                    currentSpawnInterval = 1.0f;

                if (waveTime - lastEnemySpawnTime1 >= currentSpawnInterval)
                {
                    lastEnemySpawnTime1 = waveTime;

                    if (!shooters.empty())
                    {
                        int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik, randFloat(0.f, 656.0f), shooters[randomT].get(), 1));
                        shooters[randomT]->addEnemy(enemies.back().get());
                    }
                }
            }
            else
            {
                //PRZEJŚCIE DO FALI 2
                currentWave = 2;
                waveClock.restart();
                lastEnemySpawnTime2 = 0.f;
            }
        }

        // FALA 2 
        else if (currentWave == 2)
        {
            if (waveTime < wave2Duration)
            {
                float currentSpawnInterval;

                if (waveTime < 15.0f)
                    currentSpawnInterval = 3.0f;
                else if (waveTime < 30.0f)
                    currentSpawnInterval = 2.0f;
                else
                    currentSpawnInterval = 1.0f;

                if (waveTime - lastEnemySpawnTime2 >= currentSpawnInterval)
                {
                    lastEnemySpawnTime2 = waveTime;

                    // SPAWN 2 WROGÓW
                    for (int i = 0; i < 2; i++)
                    {
                        if (!shooters.empty())
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik, randFloat(0.f, 656.0f), shooters[randomT].get(), 1));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }
                    }
                }
            }
            else
            {
                currentWave = 3;
                waveClock.restart();
                lastEnemySpawnTime3 = 0.f;
            }
        }
        
        // FALA 3
        else if (currentWave == 3)
        {
            if (waveTime < wave3Duration)
            {
                // ---- OKNA CZASOWE ----

                // 0–4, 6–9, 11–14, 16–19 → 3 wrogów co 2 sekundy
                bool tripleSpawnWindow =
                    (waveTime >= 0 && waveTime < 4) ||
                    (waveTime >= 6 && waveTime < 9) ||
                    (waveTime >= 11 && waveTime < 14) ||
                    (waveTime >= 16 && waveTime < 19);

                // 21–24, 26–29 → 6 wrogów co 1 sekundę
                bool sixSpawnWindow =
                    (waveTime >= 21 && waveTime < 24) ||
                    (waveTime >= 26 && waveTime < 29);

                // 31–34, 36–39, 41–44 → 8 wrogów co 1 sekundę
                bool eightSpawnWindow =
                    (waveTime >= 31 && waveTime < 34) ||
                    (waveTime >= 36 && waveTime < 39) ||
                    (waveTime >= 41 && waveTime < 44);

                // CO 5 SEKUND → 5 WROGÓW NARAZ
                bool fiveSpawnMoment =
                    static_cast<int>(waveTime) % 5 == 0 &&
                    waveTime - lastEnemySpawnTime3 >= 1.0f;

                // ---- SPAWN ----

                // 3 wrogów co 2 sekundy
                if (tripleSpawnWindow && waveTime - lastEnemySpawnTime3 >= 2.0f)
                {
                    lastEnemySpawnTime3 = waveTime;

                    for (int i = 0; i < 3; i++)
                    {
                        int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik,
                            randFloat(0.f, 656.0f),
                            shooters[randomT].get(),
                            1
                        ));
                        shooters[randomT]->addEnemy(enemies.back().get());
                    }
                }

                // 6 wrogów co 1 sekundę
                else if (sixSpawnWindow && waveTime - lastEnemySpawnTime3 >= 1.0f)
                {
                    lastEnemySpawnTime3 = waveTime;

                    for (int i = 0; i < 6; i++)
                    {
                        int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik,
                            randFloat(0.f, 656.0f),
                            shooters[randomT].get(),
                            1
                        ));
                        shooters[randomT]->addEnemy(enemies.back().get());
                    }
                }

                // 8 wrogów co 1 sekundę
                else if (eightSpawnWindow && waveTime - lastEnemySpawnTime3 >= 1.0f)
                {
                    lastEnemySpawnTime3 = waveTime;

                    for (int i = 0; i < 8; i++)
                    {
                        int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik,
                            randFloat(0.f, 656.0f),
                            shooters[randomT].get(),
                            1
                        ));
                        shooters[randomT]->addEnemy(enemies.back().get());
                    }
                }

                // 5 wrogów dokładnie co 5 sekund
                else if (fiveSpawnMoment)
                {
                    lastEnemySpawnTime3 = waveTime;

                    for (int i = 0; i < 5; i++)
                    {
                        int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik,
                            randFloat(0.f, 656.0f),
                            shooters[randomT].get(),
                            1
                        ));
                        shooters[randomT]->addEnemy(enemies.back().get());
                    }
                }
            }
        }



        // Czyszczenie okna
        window.clear(sf::Color::White);

        // Pobranie pozycji myszy
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Update przycisków
        jaguar1Button.update(mousePos, money);
        jaguar2Button.update(mousePos, money);
        jaguar3Button.update(mousePos, money);
        jaguar4Button.update(mousePos, money);

        // Update Shooterów
        for (auto& shooter : shooters) {
            shooter->update(dt);
        }

        // Usuwanie martwych shooterów
        auto it = std::remove_if(shooters.begin(), shooters.end(), [](const auto& s) {
            return s->getHealth() <= 0;
            });
        shooters.erase(it, shooters.end());

        // Update Enemies
        for (auto& enemy : enemies) { // Iteracja po unique_ptr
            if (shooters.empty()) enemy->setTarget(nullptr);
            if (enemy->getTarget() == nullptr) {
                // Jeśli nie ma celu, skip
                if (!shooters.empty()) {
                    int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                    enemy->setTarget(shooters[randomT].get());
                    shooters[randomT]->addEnemy(enemy.get()); // Przekazujemy wskaźnik
                }
            }
            enemy->update(dt);
        }

        // Draw
        window.draw(rectangle);
        window.draw(rect2);

        window.draw(sidePanelSprite);
        window.draw(kasa);
        window.draw(zarobki);
        window.draw(jaguar1Cost);
        window.draw(jaguar2Cost);
        window.draw(jaguar3Cost);
        window.draw(jaguar4Cost);

        // Rysowanie przycisków
        jaguar1Button.draw(window);
        jaguar2Button.draw(window);
        jaguar3Button.draw(window);
        jaguar4Button.draw(window);

        // Rysowanie shooterów
        for (const auto& shooter : shooters) {
            shooter->draw(window);
        }

        // Rysowanie enemies
        for (const auto& enemy : enemies) {
            enemy->draw(window);
        }

        window.display();
    }
}

float randFloat(float a, float b) {
    std::uniform_real_distribution<float> dist(a, b);

    return dist(rng);
}

int randInt(int a, int b) {
    std::uniform_int_distribution<> dist(a, b);
    return dist(rng);
}
