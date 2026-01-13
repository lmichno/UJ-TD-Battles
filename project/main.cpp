#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <memory>
#include <algorithm>

#include "shooter.hpp"
#include "enemy.hpp"
#include "button.hpp"
#include "bullet.hpp"
#include "menu.hpp"

std::mt19937 rng(std::random_device{}());

float randFloat(float a, float b);
int randInt(int a, int b);

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "UJ TD 1");

    while (window.isOpen()) 
    {
        //MENU 
        MenuResult result = showMenu(window);
        int lvl = result.lvl;
        int difficulty = result.difficulty;
        HordMode(difficulty);
        sf::sleep(sf::milliseconds(150));
        window.requestFocus();

        // RESET ZMIENNYCH 
        sf::Clock clock;
        window.setFramerateLimit(60);

        std::vector<std::unique_ptr<Shooter>> shooters;
        std::vector<std::unique_ptr<Enemy>> enemies;

        int money = 0;
        int moneyAdd = 5;
        float moneyTimer = 0.0f;

        const float gameOverLineX = 190.f;
        bool gameOver = false;

        int currentWave = 1;

        float wave1Duration = 45.0f;
        float lastEnemySpawnTime1 = -5.0f;

        float wave2Duration = 45.0f;
        float lastEnemySpawnTime2 = -3.0f;

        float wave3Duration = 45.0f;
        float lastEnemySpawnTime3 = -2.0f;

        float wave4Duration = 45.0f;
        float lastEnemySpawnTime4 = -2.0f;

        float wave5Duration = 45.0f;
        float lastEnemySpawnTime5 = -2.0f;

        float wave6Duration = 45.0f;
        float lastEnemySpawnTime6 = -2.0f;

        float wave7Duration = 30.0f;
        float lastEnemySpawnTime7 = -1.0f;

        sf::Clock waveClock;

        //FONTY
        sf::Font mediumGothic("ScienceGothic-Medium.ttf");
        //NAPIS FALI
        sf::Text waveText(mediumGothic);
        waveText.setCharacterSize(48);
        waveText.setFillColor(sf::Color::Red);
        waveText.setOutlineThickness(2);
        waveText.setOutlineColor(sf::Color::Blue);
        waveText.setString("FALA 1");
        waveText.setPosition(sf::Vector2f(
            window.getSize().x * 0.5f - 80.f,
            window.getSize().y - 70.f 
        ));

        

        //PANEL BOCZNY
        Button jaguar1Button(12, 115, 100, 125, 15);
        Button jaguar2Button(12, 243, 100, 125, 30);
        Button jaguar3Button(12, 371, 100, 125, 60);
        Button jaguar4Button(12, 504, 100, 125, 120);

        sf::Text text(mediumGothic);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::Black);

        sf::Text kasa = text;
        kasa.setFillColor(sf::Color::Yellow);
        kasa.setOutlineThickness(1);
        kasa.setOutlineColor(sf::Color::Black);
        kasa.setPosition({ 20, 38 });

        sf::Text zarobki = text;
        zarobki.setString(std::to_string(moneyAdd) + " $/s");
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

        //TEKSTURY
        sf::Texture jaguar1("jaguar1.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
        sf::Texture jaguar2("wrog2.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
        sf::Texture jaguar3("wrog3.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
        sf::Texture ludzik("jaguar1.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
        sf::Texture ludzik2("wrog2.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
        sf::Texture ludzik3("wrog3.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));
        sf::Texture bullet("bullet.png", false, sf::IntRect({ 0, 0 }, { 26, 8 }));
        sf::Texture sidePanel("sidePanel.png");

        sf::Sprite sidePanelSprite(sidePanel);

        //NA POCZATKU SHOOTERZY
        for (int i = 0; i < 3; i++)
        {
            shooters.push_back(std::make_unique<Shooter>(
                jaguar1, randFloat(130.f, 170.f), randFloat(0.f, 656.0f)));
        }

        sf::RectangleShape rectangle({ 5, 720 });
        rectangle.setFillColor(sf::Color::Black);
        rectangle.setPosition({ 190, 0 });

        sf::RectangleShape rect2({ 2, 720 });
        rect2.setFillColor(sf::Color::Black);
        rect2.setPosition({ 126, 0 });

        waveClock.restart();

        while (window.isOpen() && !gameOver)
        {
            // ZAKUP
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();

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

                    if (jaguar1Button.onClicked(money))
                        shooters.push_back(std::make_unique<Shooter>(
                            jaguar1, randFloat(130.f, 170.f), randFloat(0.f, 656.0f)));

                    if (jaguar2Button.onClicked(money))
                        shooters.push_back(std::make_unique<Shooter>(
                            jaguar2, randFloat(130.f, 170.f), randFloat(0.f, 656.0f)));

                    if (jaguar3Button.onClicked(money))
                        shooters.push_back(std::make_unique<Shooter>(
                            jaguar3, randFloat(130.f, 170.f), randFloat(0.f, 656.0f)));

                    jaguar4Button.onClicked(money);

                    kasa.setString(std::to_string(money));
                }

            }


            // DELTA TIME
            float dt = clock.restart().asSeconds();
            float waveTime = waveClock.getElapsedTime().asSeconds();

            // MONEY CASH
            moneyTimer += dt;
            while (moneyTimer >= 3.0f)
            {
                money += moneyAdd;
                moneyTimer = 0.0f;
            }
            kasa.setString(std::to_string(money));


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
                                ludzik, randFloat(0.f, 656.0f), shooters[randomT].get(), 0));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }
                    }
                }
                else
                {
                    //PRZEJŚCIE DO FALI 2
                    currentWave = 2;
                    waveClock.restart();
                    waveText.setString("FALA 2");
                    lastEnemySpawnTime2 = -3.0f;
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

                        if (!shooters.empty())
                        {
                            int randomT1 = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik, randFloat(0.f, 656.0f), shooters[randomT1].get(), 0));
                            shooters[randomT1]->addEnemy(enemies.back().get());

                            int randomT2 = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0.f, 656.0f), shooters[randomT2].get(), 1));
                            shooters[randomT2]->addEnemy(enemies.back().get());
                        }

                    }
                }
                else
                {
                    currentWave = 3;
                    waveClock.restart();
                    waveText.setString("FALA 3");
                    lastEnemySpawnTime3 = -2.0f;
                }
            }

            // FALA 3
            else if (currentWave == 3)
            {
                if (shooters.empty())
                    continue;

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

                        // 2x ludzik2
                        for (int i = 0; i < 2; i++)
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0.f, 656.0f), shooters[randomT].get(), 1));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }

                        // 1x ludzik1
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik, randFloat(0.f, 656.0f), shooters[randomT].get(), 0));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }


                    }

                    // 6 wrogów co 1 sekundę
                    else if (sixSpawnWindow && waveTime - lastEnemySpawnTime3 >= 1.0f)
                    {
                        lastEnemySpawnTime3 = waveTime;

                        // 4x ludzik2
                        for (int i = 0; i < 4; i++)
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0.f, 656.0f), shooters[randomT].get(), 1));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }

                        // 2x ludzik3
                        for (int i = 0; i < 2; i++)
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik3, randFloat(0.f, 656.0f), shooters[randomT].get(), 2));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }

                    }

                    // 8 wrogów co 1 sekundę
                    else if (eightSpawnWindow && waveTime - lastEnemySpawnTime3 >= 1.0f)
                    {
                        lastEnemySpawnTime3 = waveTime;

                        // 5x ludzik3
                        for (int i = 0; i < 5; i++)
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik3, randFloat(0.f, 656.0f), shooters[randomT].get(), 2));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }

                        // 3x ludzik2
                        for (int i = 0; i < 3; i++)
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0.f, 656.0f), shooters[randomT].get(), 1));
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
                            enemies.push_back(std::make_unique<Enemy>(ludzik3, randFloat(0.f, 656.0f), shooters[randomT].get(), 2));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }
                    }
                }
                if (waveTime >= wave3Duration)
                {
                    currentWave = 4;
                    waveClock.restart();
                    waveText.setString("FALA 4");
                    lastEnemySpawnTime4 = -2.0f;
                    continue;
                }
            }

            //FALA 4
            else if (currentWave == 4)
            {
                if (shooters.empty())
                    continue;

                if (waveTime < 45.0f)
                {
                    // 0–15: co 3s → 2x ludzik2 + 1x ludzik1
                    if (waveTime < 15 && waveTime - lastEnemySpawnTime4 >= 3.0f)
                    {
                        lastEnemySpawnTime4 = waveTime;

                        // 4x ludzik2
                        for (int i = 0; i < 4; i++)
                        {
                            int r = randInt(0, shooters.size() - 1);
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0, 656), shooters[r].get(), 1));
                            shooters[r]->addEnemy(enemies.back().get());
                        }

                        // 1x ludzik1
                        {
                            int r = randInt(0, shooters.size() - 1);
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik, randFloat(0, 656), shooters[r].get(), 0));
                            shooters[r]->addEnemy(enemies.back().get());
                        }
                    }

                    // 15–30: co 4s → 3x ludzik2 + 3x ludzik3
                    else if (waveTime >= 15 && waveTime < 30 &&
                        waveTime - lastEnemySpawnTime4 >= 2.0f)
                    {
                        lastEnemySpawnTime4 = waveTime;

                        for (int i = 0; i < 3; i++)
                        {
                            int r = randInt(0, shooters.size() - 1);
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0, 656), shooters[r].get(), 1));
                            shooters[r]->addEnemy(enemies.back().get());
                        }

                        for (int i = 0; i < 3; i++)
                        {
                            int r = randInt(0, shooters.size() - 1);
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik3, randFloat(0, 656), shooters[r].get(), 2));
                            shooters[r]->addEnemy(enemies.back().get());
                        }
                    }

                    // 30–45: co 3s → 2x ludzik3 + 3x ludzik2
                    else if (waveTime >= 30 &&
                        waveTime - lastEnemySpawnTime4 >= 3.0f)
                    {
                        lastEnemySpawnTime4 = waveTime;

                        for (int i = 0; i < 2; i++)
                        {
                            int r = randInt(0, shooters.size() - 1);
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik3, randFloat(0, 656), shooters[r].get(), 2));
                            shooters[r]->addEnemy(enemies.back().get());
                        }

                        for (int i = 0; i < 3; i++)
                        {
                            int r = randInt(0, shooters.size() - 1);
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0, 656), shooters[r].get(), 1));
                            shooters[r]->addEnemy(enemies.back().get());
                        }
                    }
                    if (waveTime >= wave3Duration)
                    {
                        currentWave = 5;
                        waveClock.restart();
                        waveText.setString("FALA 5");
                        lastEnemySpawnTime5 = -2.0f;
                        continue;
                    }
                }
            }

            //FALA 5
            else if (currentWave == 5)
            {
                if (shooters.empty())
                    continue;

                if (waveTime < 45.0f &&
                    waveTime - lastEnemySpawnTime5 >= 3.0f)
                {
                    lastEnemySpawnTime5 = waveTime;

                    // 4x ludzik3
                    for (int i = 0; i < 4; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik3, randFloat(0, 656), shooters[r].get(), 2));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

                    // 3x ludzik2 i 4x ludzik3
                    for (int i = 0; i < 3; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik2, randFloat(0, 656), shooters[r].get(), 1));
                        shooters[r]->addEnemy(enemies.back().get());
                    }
                    for (int i = 0; i < 4; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik3, randFloat(0, 656), shooters[r].get(), 2));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

                    // 1x ludzik1 (distraction)
                    int r = randInt(0, shooters.size() - 1);
                    enemies.push_back(std::make_unique<Enemy>(
                        ludzik, randFloat(0, 656), shooters[r].get(), 0));
                    shooters[r]->addEnemy(enemies.back().get());
                }
                if (waveTime >= wave3Duration)
                {
                    currentWave = 6;
                    waveClock.restart();
                    waveText.setString("FALA 6");
                    lastEnemySpawnTime6 = -2.0f;
                    continue;
                }
            }

            //FALA 6
            else if (currentWave == 6)
            {
                if (shooters.empty())
                    continue;

                if (waveTime < 45.0f &&
                    waveTime - lastEnemySpawnTime6 >= 4.0f)
                {
                    lastEnemySpawnTime6 = waveTime;

                    // 6x ludzik3
                    for (int i = 0; i < 6; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik3, randFloat(0, 656), shooters[r].get(), 2));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

                    // 2x ludzik2
                    for (int i = 0; i < 2; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik2, randFloat(0, 656), shooters[r].get(), 1));
                        shooters[r]->addEnemy(enemies.back().get());
                    }


                }
                else
                {
                    currentWave = 7;
                    waveClock.restart();
                    waveText.setString("FALA 7");
                    lastEnemySpawnTime7 = -1.0f;
                }

            }

            //FALA 7
            else if (currentWave == 7)
            {
                if (shooters.empty())
                    continue;


                if (waveTime >= wave7Duration)
                    continue;

                // ---- OKNA CZASOWE ----

                // 0–10s → 4 wrogów co 1.5s (3x ludzik3 + 1x ludzik2)
                bool earlyPressure = waveTime < 10.0f;

                // 10–20s → 6 wrogów co 1s (4x ludzik3 + 2x ludzik2)
                bool midChaos = waveTime >= 10.0f && waveTime < 20.0f;

                // 20–30s → 10 wrogów co 1s (7x ludzik3 + 3x ludzik2)
                bool finalStorm = waveTime >= 20.0f;

                // ---- SPAWN ----

                //EARLY
                if (earlyPressure && waveTime - lastEnemySpawnTime7 >= 1.5f)
                {
                    lastEnemySpawnTime7 = waveTime;

                    for (int i = 0; i < 3; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik3, randFloat(0.f, 656.f), shooters[r].get(), 2));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

                    int r = randInt(0, shooters.size() - 1);
                    enemies.push_back(std::make_unique<Enemy>(
                        ludzik2, randFloat(0.f, 656.f), shooters[r].get(), 1));
                    shooters[r]->addEnemy(enemies.back().get());
                }

                // MID
                else if (midChaos && waveTime - lastEnemySpawnTime7 >= 1.0f)
                {
                    lastEnemySpawnTime7 = waveTime;

                    for (int i = 0; i < 4; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik3, randFloat(0.f, 656.f), shooters[r].get(), 2));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

                    for (int i = 0; i < 2; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik2, randFloat(0.f, 656.f), shooters[r].get(), 1));
                        shooters[r]->addEnemy(enemies.back().get());
                    }
                }

                //FINAL
                else if (finalStorm && waveTime - lastEnemySpawnTime7 >= 1.0f)
                {
                    lastEnemySpawnTime7 = waveTime;

                    for (int i = 0; i < 7; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik3, randFloat(0.f, 656.f), shooters[r].get(), 2));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

                    for (int i = 0; i < 3; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik2, randFloat(0.f, 656.f), shooters[r].get(), 1));
                        shooters[r]->addEnemy(enemies.back().get());
                    }
                }
            }







            // Czyszczenie okna
            window.clear(sf::Color::White);

            // Pobranie pozycji myszy
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));


            // ======= UPDATE SHOOTERÓW =======
            for (auto& shooter : shooters)
            {
                shooter->update(dt);

                Enemy* target = shooter->findNearestEnemy(enemies);
                if (target)
                    shooter->shoot(target, bullet);

                shooter->updateBullets(dt, enemies);
            }

            shooters.erase(
                std::remove_if(shooters.begin(), shooters.end(),
                    [](const auto& s) { return s->getHealth() <= 0; }),
                shooters.end());

            // ======= UPDATE ENEMIES =======
            for (auto& enemy : enemies)
            {
                if (shooters.empty())
                    enemy->setTarget(nullptr);

                if (enemy->getTarget() == nullptr && !shooters.empty())
                {
                    int r = randInt(0, shooters.size() - 1);
                    enemy->setTarget(shooters[r].get());
                    shooters[r]->addEnemy(enemy.get());
                }

                enemy->update(dt);

                if (enemy->getPosition().x < gameOverLineX)
                {
                    gameOver = true;
                    break;
                }
            }

            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [](const auto& e) { return !e->isAlive(); }),
                enemies.end());
            for (auto& shooter : shooters)
                shooter->cleanupEnemies();


            //RYSOWANIE


            window.clear(sf::Color::White);

            mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            jaguar1Button.update(mousePos, money);
            jaguar2Button.update(mousePos, money);
            jaguar3Button.update(mousePos, money);
            jaguar4Button.update(mousePos, money);

            window.draw(rectangle);
            window.draw(rect2);

            window.draw(sidePanelSprite);
            window.draw(kasa);
            window.draw(zarobki);
            window.draw(jaguar1Cost);
            window.draw(jaguar2Cost);
            window.draw(jaguar3Cost);
            window.draw(jaguar4Cost);

            jaguar1Button.draw(window);
            jaguar2Button.draw(window);
            jaguar3Button.draw(window);
            jaguar4Button.draw(window);

            for (const auto& shooter : shooters)
            {
                shooter->draw(window);
                shooter->drawBullets(window);
            }

            for (const auto& enemy : enemies)
                enemy->draw(window);
            //nazwa fali
            window.draw(waveText);

            window.display();
        }

        // GAME OVER 
        sf::Clock overClock;
        while (overClock.getElapsedTime().asSeconds() < 2.0f && window.isOpen())
        {
            window.clear(sf::Color::Black);

            sf::Text gameOverText(mediumGothic);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(72);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition({ 400, 300 });

            window.draw(gameOverText);
            window.display();
        }

    }
}

float randFloat(float a, float b)
{
    std::uniform_real_distribution<float> dist(a, b);
    return dist(rng);
}

int randInt(int a, int b)
{
    std::uniform_int_distribution<int> dist(a, b);
    return dist(rng);
}

