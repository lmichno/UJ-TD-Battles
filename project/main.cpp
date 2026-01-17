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
    // Główne operacje
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "AGH Defenders"); // Utworzenie okna
   

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

        int money = 200;
        int moneyAdd = 5;
        float moneyTimer = 0.0f;

        bool bombaActive = false;
        sf::Clock bombaClock;
        sf::RectangleShape bombaFlash;

        //SKRZYNIA Z KASA
        sf::Texture chestTexture("skrzynia.png"); 
        sf::Sprite chestSprite(chestTexture);

        bool chestActive = false;
        bool chestFalling = false;

        sf::Clock chestSpawnClock;
        sf::Clock chestLifeClock;

        float chestSpawnDelay = randFloat(12.f, 25.f);
        float chestLifetime = 0.f;

        float chestFallSpeed = 300.f; 
 
        const float playAreaLeft = 200.f;
        const float playAreaRight = 1220.f;
        const float playAreaMinGroundY = 100.f; 
        const float playAreaMaxGroundY = 560.f; 
        float chestTargetY = 0.f;


        int chestReward = 100;


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
        float lastEnemySpawnTime4Boss = -10.0f;


        float wave5Duration = 45.0f;
        float lastEnemySpawnTime5 = -2.0f;
        float lastEnemySpawnTime5Boss = -100.f;

        float wave6Duration = 45.0f;
        float lastEnemySpawnTime6 = -2.0f;
        float lastEnemySpawnTime6Boss = -100.f;


        float wave7Duration = 30.0f;
        float lastEnemySpawnTime7 = -1.0f;
        float lastEnemySpawnTime7Boss = -100.f;

        bool wave7Spawning = true;

        bool gameWon = false;
       



        sf::Clock waveClock;

        //FONTY
        sf::Font mediumGothic("ScienceGothic-Medium.ttf");
        //NAPIS FALI
        sf::Text waveText(mediumGothic);
        waveText.setCharacterSize(38);
        waveText.setFillColor(sf::Color(53, 57, 40));
        waveText.setOutlineThickness(2);
        waveText.setOutlineColor(sf::Color::Black);
        waveText.setString("FALA 1");
        waveText.setPosition(sf::Vector2f(1100,670));

        

        //PANEL BOCZNY
        Button jaguar1Button(12, 115, 100, 125, 15);
        Button jaguar2Button(12, 243, 100, 125, 30);
        Button jaguar3Button(12, 371, 100, 125, 60);
        Button jaguar4Button(12, 504, 100, 125, 120);
        Button bombaButton(9, 630, 106, 42, 350);

        sf::Text text(mediumGothic);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::Black);

        sf::Text kasa = text;
        kasa.setFillColor(sf::Color::Yellow);
        kasa.setOutlineThickness(1.8f);
        kasa.setOutlineColor(sf::Color::Black);
        kasa.setPosition({ 20, 38 });

        sf::Text zarobki = text;
        zarobki.setString(std::to_string(moneyAdd) + " $/s");
        zarobki.setPosition({ 20, 86 });
        zarobki.setFillColor(sf::Color(134, 172, 19));
        zarobki.setOutlineThickness(1.8f);
        zarobki.setOutlineColor(sf::Color::Black);

        sf::Text jaguar1Cost = text;
        jaguar1Cost.setCharacterSize(20);
        jaguar1Cost.setFillColor(sf::Color(134, 172, 19));
        jaguar1Cost.setOutlineThickness(1.8f);

        sf::Text jaguar2Cost = jaguar1Cost;
        sf::Text jaguar3Cost = jaguar1Cost;
        sf::Text jaguar4Cost = text;
        sf::Text bombaCost = text;
        sf::Text winText = text;
        jaguar4Cost.setCharacterSize(18);
        jaguar4Cost.setFillColor(sf::Color(134, 172, 19));
        jaguar4Cost.setOutlineThickness(1.8f);

        jaguar1Cost.setString("15");
        jaguar1Cost.setPosition({ 70, 212 });
        jaguar2Cost.setString("30");
        jaguar2Cost.setPosition({ 70, 340 });
        jaguar3Cost.setString("60");
        jaguar3Cost.setPosition({ 70, 468 });
        jaguar4Cost.setString("120");
        jaguar4Cost.setPosition({ 68, 603 });

        bombaCost.setOutlineThickness(1.8f);
        bombaCost.setCharacterSize(14);
        bombaCost.setFillColor(sf::Color(134, 172, 19));
        bombaCost.setString("350");
        bombaCost.setPosition({ 75, 643 });

        
        winText.setString("You win!");
        winText.setCharacterSize(120);
        winText.setFillColor(sf::Color::White);
        winText.setStyle(sf::Text::Bold);
        winText.setPosition({ 1280 / 2.f - 300.f, 720 / 2.f - 80.f });

        
        //BOMBA
        bombaFlash.setSize(sf::Vector2f(1280, 720));
        bombaFlash.setFillColor(sf::Color(255, 255, 255, 255));
        

        //TEKSTURY
        sf::Texture jaguar1("jaguar1.png");
        sf::Texture jaguar2("jaguar2.png");
        sf::Texture jaguar3("jaguar3.png");
        sf::Texture jaguar4("jaguar4.png");
        sf::Texture ludzik("wrog1.png");
        sf::Texture ludzik2("wrog2.png");
        sf::Texture ludzik3("wrog3.png");
        sf::Texture ludzik4("wrog4.png");
        sf::Texture backgroundTexture1;
        backgroundTexture1.loadFromFile("background.png");
        sf::Texture backgroundTexture2;
        backgroundTexture2.loadFromFile("background1.png");
        sf::Texture backgroundTexture3;
        backgroundTexture3.loadFromFile("background2.png");

        sf::Sprite backgroundSprite1(backgroundTexture1);
        backgroundSprite1.setScale({ 1280.f / backgroundTexture1.getSize().x,720.f / backgroundTexture1.getSize().y });
        sf::Sprite backgroundSprite2(backgroundTexture2);
        backgroundSprite2.setScale({ 1280.f / backgroundTexture2.getSize().x,720.f / backgroundTexture2.getSize().y });
        sf::Sprite backgroundSprite3(backgroundTexture3);
        backgroundSprite3.setScale({ 1280.f / backgroundTexture3.getSize().x,720.f / backgroundTexture3.getSize().y });

        sf::Texture bullet("bullet.png", false, sf::IntRect({ 0, 0 }, { 26, 8 }));
        sf::Texture sidePanel("sidePanel.png");

        sf::Sprite sidePanelSprite(sidePanel);

        //NA POCZATKU SHOOTERZY
        for (int i = 0; i < 0; i++)
        {
            shooters.push_back(std::make_unique<Shooter>(
                jaguar1, randFloat(130.f, 170.f), randFloat(0.f, 656.0f), 0));
        }

       

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

                    if (chestActive && !chestFalling &&
                        mousePressed->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2f mousePos =
                            window.mapPixelToCoords({ mousePressed->position.x, mousePressed->position.y });

                        if (chestSprite.getGlobalBounds().contains(mousePos))
                        {
                            money += chestReward;

                            chestActive = false;
                            chestSpawnClock.restart();
                        }
                    }

                }

                if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
                {
                    jaguar1Button.onMouseReleased();
                    jaguar2Button.onMouseReleased();
                    jaguar3Button.onMouseReleased();
                    jaguar4Button.onMouseReleased();
                    bombaButton.onMouseReleased();

                    if (jaguar1Button.onClicked(money))
                        shooters.push_back(std::make_unique<Shooter>(
                            jaguar1, randFloat(130.f, 170.f), randFloat(0.f, 656.0f), 0));

                    if (jaguar2Button.onClicked(money))
                        shooters.push_back(std::make_unique<Shooter>(
                            jaguar2, randFloat(130.f, 170.f), randFloat(0.f, 656.0f), 1));

                    if (jaguar3Button.onClicked(money))
                        shooters.push_back(std::make_unique<Shooter>(
                            jaguar3, randFloat(130.f, 170.f), randFloat(0.f, 656.0f), 2));

                    if (jaguar4Button.onClicked(money))
                        shooters.push_back(std::make_unique<Shooter>(
                            jaguar4, randFloat(130.f, 170.f), randFloat(0.f, 656.0f), 3));

                    if (bombaButton.onClicked(money) && !bombaActive)
                    {
                        bombaActive = true;
                        bombaClock.restart();
                        enemies.clear();
                    }

                    
                    

                    kasa.setString(std::to_string(money));
                }

            }


            // DELTA TIME
            float dt = clock.restart().asSeconds();
            float waveTime = waveClock.getElapsedTime().asSeconds();

            //BOMBA
            if (bombaActive)
            {
                if (bombaClock.getElapsedTime().asSeconds() >= 1.0f)
                {
                    bombaActive = false;
                }
            }

            //SPAWN SKRZYNKI
            if (!chestActive && chestSpawnClock.getElapsedTime().asSeconds() >= chestSpawnDelay)
            {
                chestActive = true;
                chestFalling = true;

                chestSprite.setPosition({
                 randFloat(playAreaLeft, playAreaRight - 27.f), 
                   -30.f
                    });


                chestSpawnDelay = randFloat(12.f, 25.f);
                chestTargetY = randFloat(playAreaMinGroundY, playAreaMaxGroundY);

            }
            
        // ---------- SPADANIE ----------
            if (chestActive && chestFalling)
            {
                chestSprite.move({ 0.f, chestFallSpeed * dt });

                if (chestSprite.getPosition().y >= chestTargetY)
                {
                    chestSprite.setPosition({
                        chestSprite.getPosition().x,
                        chestTargetY
                        });


                    chestFalling = false;
                    chestLifetime = randFloat(5.f, 10.f);
                    chestLifeClock.restart();
                }
            }

        // ---------- CZAS NA KLIK ----------
        if (chestActive && !chestFalling &&
            chestLifeClock.getElapsedTime().asSeconds() >= chestLifetime)
        {
            chestActive = false;
            chestSpawnClock.restart();
        }


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
                if (!shooters.empty())
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

                    // 4 wrogów co 1 sekundę
                    else if (sixSpawnWindow && waveTime - lastEnemySpawnTime3 >= 1.0f)
                    {
                        lastEnemySpawnTime3 = waveTime;

                        // 3x ludzik2
                        for (int i = 0; i < 3; i++)
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik2, randFloat(0.f, 656.0f), shooters[randomT].get(), 1));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }

                        // 1x ludzik3
                        for (int i = 0; i < 1; i++)
                        {
                            int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik3, randFloat(0.f, 656.0f), shooters[randomT].get(), 2));
                            shooters[randomT]->addEnemy(enemies.back().get());
                        }

                    }

                    // 6 wrogów co 1 sekundę
                    else if (eightSpawnWindow && waveTime - lastEnemySpawnTime3 >= 1.0f)
                    {
                        lastEnemySpawnTime3 = waveTime;

                        // 3x ludzik3
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
                    lastEnemySpawnTime4Boss = -10.0f;
                    continue;
                }
                } // End if !shooters.empty()
            }

            //FALA 4
            else if (currentWave == 4)
            {
                if (!shooters.empty())
                {
                    if (waveTime - lastEnemySpawnTime4Boss >= 10.0f)
                    {
                        lastEnemySpawnTime4Boss = waveTime;

                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik4,
                            randFloat(0.f, 656.f),
                            shooters[r].get(),
                            3));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

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
                    
                    
                    
                }
                if (waveTime >= wave4Duration)
                {
                    currentWave = 5;
                    waveClock.restart();
                    waveText.setString("FALA 5");
                    lastEnemySpawnTime5 = -2.0f;
                    

                    continue;
                }
                } // End if !shooters.empty()
            }

            //FALA 5
            else if (currentWave == 5)
            {
                if (!shooters.empty())
                {
                    // ===== 2x KOPARKA – FALA 5 (co 10s) =====
                    if (waveTime < wave5Duration &&
                        waveTime - lastEnemySpawnTime5Boss >= 10.f)
                    {
                        lastEnemySpawnTime5Boss = waveTime;

                        for (int i = 0; i < 2; i++)
                        {
                            int r = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik4,
                                randFloat(0.f, 656.f),
                                shooters[r].get(),
                                3));
                            shooters[r]->addEnemy(enemies.back().get());
                        }
                    }

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
                if (waveTime >= wave5Duration)
                {
                    currentWave = 6;
                    waveClock.restart();
                    waveText.setString("FALA 6");
                    lastEnemySpawnTime6 = -2.0f;
                    continue; 
                }
                } // End if !shooters.empty()
            }

            //FALA 6
            else if (currentWave == 6)
            {
                if (!shooters.empty())
                {
                    if (waveTime < wave6Duration &&
                        waveTime - lastEnemySpawnTime6Boss >= 15.f)
                    {
                        lastEnemySpawnTime6Boss = waveTime;

                        for (int i = 0; i < 3; i++)
                        {
                            int r = randInt(0, static_cast<int>(shooters.size() - 1));
                            enemies.push_back(std::make_unique<Enemy>(
                                ludzik4,
                                randFloat(0.f, 656.f),
                                shooters[r].get(),
                                3));
                            shooters[r]->addEnemy(enemies.back().get());
                        }
                    }

                if (waveTime < 45.0f &&
                    waveTime - lastEnemySpawnTime6 >= 4.0f)
                {
                    lastEnemySpawnTime6 = waveTime;

                    // 5x ludzik3
                    for (int i = 0; i < 5; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik3, randFloat(0, 656), shooters[r].get(), 2));
                        shooters[r]->addEnemy(enemies.back().get());
                    }

                    // 1x ludzik2
                    for (int i = 0; i < 1; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik2, randFloat(0, 656), shooters[r].get(), 1));
                        shooters[r]->addEnemy(enemies.back().get());
                    }


                }
                
                if (waveTime >= wave6Duration) {
                    currentWave = 7;
                    waveClock.restart();
                    waveText.setString("FALA 7");
                    lastEnemySpawnTime7 = -1.0f;
                }
                } // End if !shooters.empty()

            }

            //FALA 7
            else if (currentWave == 7)
            {
                if (!shooters.empty())
                {


                if (waveTime >= wave7Duration)
                {
                    wave7Spawning = false;
                }

                // ---- OKNA CZASOWE ----

                // 0–10s → 2 wrogów co 1.5s (3x ludzik3 + 1x ludzik2) i co 5s ludzik4
                bool earlyPressure = waveTime < 10.0f;

                // 10–20s → 6 wrogów co 1s (4x ludzik3 + 2x ludzik2) i co 3 s ludzik4
                bool midChaos = waveTime >= 10.0f && waveTime < 20.0f;

                // 20–30s → 10 wrogów co 1s (7x ludzik3 + 3x ludzik2) i co 2s ludzik4
                bool finalStorm = waveTime >= 20.0f && waveTime < wave7Duration;


                // ---- SPAWN ----

                //EARLY
                if  (wave7Spawning && waveTime < 10.f &&
                    waveTime - lastEnemySpawnTime7Boss >= 5.f)
                {
                    lastEnemySpawnTime7Boss = waveTime;

                    for (int i = 0; i < 2; i++)
                    {
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik4,
                            randFloat(0.f, 656.f),
                            shooters[r].get(),
                            3));
                        shooters[r]->addEnemy(enemies.back().get());
                    }
                }

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
                 if (wave7Spawning && waveTime >= 10.f && waveTime < 20.f &&
                    waveTime - lastEnemySpawnTime7Boss >= 3.f)
                {
                    lastEnemySpawnTime7Boss = waveTime;

                    
                        int r = randInt(0, shooters.size() - 1);
                        enemies.push_back(std::make_unique<Enemy>(
                            ludzik4,
                            randFloat(0.f, 656.f),
                            shooters[r].get(),
                            3));
                        shooters[r]->addEnemy(enemies.back().get());
                    
                }
                 if (midChaos && waveTime - lastEnemySpawnTime7 >= 1.0f)
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
                 if (wave7Spawning && waveTime >= 20.f &&
                    waveTime - lastEnemySpawnTime7Boss >= 2.f)
                {
                    lastEnemySpawnTime7Boss = waveTime;

                    int r = randInt(0, shooters.size() - 1);
                    enemies.push_back(std::make_unique<Enemy>(
                        ludzik4,
                        randFloat(0.f, 656.f),
                        shooters[r].get(),
                        3));
                    shooters[r]->addEnemy(enemies.back().get());
                }

                 if (finalStorm && waveTime - lastEnemySpawnTime7 >= 1.0f)
                {
                    lastEnemySpawnTime7 = waveTime;

                    for (int i = 0; i < 4; i++)
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
                if (!wave7Spawning && enemies.empty())
                {
                    gameWon = true;
                }
                } // End !shooters.empty()
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
                
                if (enemy->isReadyToShoot())
                {
                    enemy->shoot(bullet);
                    enemy->resetReadyToShoot();
                }

                if (enemy->getPosition().x < gameOverLineX)
                {
                    gameOver = true;
                    break;
                }
            }

            for (auto& shooter : shooters)
                shooter->cleanupEnemies();

            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [](const auto& e) { return !e->isAlive(); }),
                enemies.end());


            //RYSOWANIE

            mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            switch(lvl)
            {
                case 1:
                    window.draw(backgroundSprite1);
                    break;
                case 2:
                    window.draw(backgroundSprite2);
                    break;
                case 3:
                    window.draw(backgroundSprite3);
                    break;
                default:
                    window.draw(backgroundSprite1);
                    break;
			}
            
            jaguar1Button.update(mousePos, money);
            jaguar2Button.update(mousePos, money);
            jaguar3Button.update(mousePos, money);
            jaguar4Button.update(mousePos, money);
            bombaButton.update(mousePos, money);

           

            window.draw(sidePanelSprite);
            window.draw(kasa);
            window.draw(zarobki);
            window.draw(jaguar1Cost);
            window.draw(jaguar2Cost);
            window.draw(jaguar3Cost);
            window.draw(jaguar4Cost);
            window.draw(bombaCost);


            jaguar1Button.draw(window);
            jaguar2Button.draw(window);
            jaguar3Button.draw(window);
            jaguar4Button.draw(window);
            bombaButton.draw(window);
           

            for (const auto& shooter : shooters)
            {
                shooter->draw(window);
                shooter->drawBullets(window);
            }

            for (const auto& enemy : enemies)
            {
                enemy->draw(window);
                enemy->drawBullets(window);
            }
            //nazwa fali
            window.draw(waveText);

            if (chestActive)
                window.draw(chestSprite);

             if (bombaActive)
            {
                window.draw(bombaFlash);
            }
             if (gameWon)
             {
                 window.draw(winText);
             }

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

