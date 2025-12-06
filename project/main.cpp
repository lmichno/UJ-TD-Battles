#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

#include "shooter.hpp"
#include "enemy.hpp"
#include "button.hpp"



std::mt19937 rng(std::random_device{}()); // Globalny generator losowy

float randFloat(float a, float b);
int randInt(int a, int b);

int main()
{
    // G³ówne operacje
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "UJ TD 1"); // Utworzenie okna

    sf::Clock clock; // Zegar do deltaTime

    window.setFramerateLimit(60); // Ustawienie 60fps

    // Zmienne
    std::vector<Shooter> shooters;
	std::vector<Enemy> enemies;
    int money = 100;
    int moneyAdd = 15;


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
	sf::Texture sidePanel("sidePanel.png");

    //Sprite
	sf::Sprite sidePanelSprite(sidePanel);


    // Operacje wstêpne
    for (int i = 0; i < 10; i++)
    {
        shooters.emplace_back(jaguar1, randFloat(130.f, 170.f), randFloat(0.f, 656.0f)); // generowanie shooterów, 656 aby nie wychodzi³ poza ekran (720-32*2)
    }
    for (int i = 0; i < 10; i++)
    {
        int randomT = randInt(0, static_cast<int>(shooters.size() - 1));
		enemies.emplace_back(ludzik, randFloat(0.f, 656.0f), &shooters[randomT]); // Jako target podajemy losowego shootera
		shooters[randomT].addEnemy(&enemies[i]);
    }


    //TEMP
    sf::RectangleShape rectangle({ 5, 720 });
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setPosition({ 190, 0 });
    sf::RectangleShape rect2({ 2, 720 });
    rect2.setFillColor(sf::Color::Black);
    rect2.setPosition({ 126, 0 });


    while (window.isOpen()) // G³ówna pêtla aplikacji
    {
        while (const std::optional event = window.pollEvent()) // Sprawdzanie eventów
        {
            if (event->is<sf::Event::Closed>()) // Obs³uga zamkniêcia okna
                window.close();

            // Obs³uga klikniêæ myszy
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

        // Wyliczanie dt (czas miêdzy kolejnymi klatkami)
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Czyszczenie okna
        window.clear(sf::Color::White);

        // Pobranie pozycji myszy
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Update przycisków
        jaguar1Button.update(mousePos, money);
        jaguar2Button.update(mousePos, money);
        jaguar3Button.update(mousePos, money);
        jaguar4Button.update(mousePos, money);

        //Update
        /*for (Shooter& shooter : shooters) {
            shooter.update(dt);
        }
        for (Enemy& enemy : enemies) {
			enemy.update(dt);
		}*/

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

        for (Shooter& shooter : shooters) {
            shooter.update(dt);
            shooter.draw(window);
        }
        for (Enemy& enemy : enemies) {
            enemy.update(dt);
            enemy.draw(window);
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


