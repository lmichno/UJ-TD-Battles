#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

#include "shooter.hpp"



std::mt19937 rng(std::random_device{}()); // Globalny generator losowy

float randFloat(float a, float b);

int main()
{
    // G³ówne operacje
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "UJ TD 1"); // Utworzenie okna

    sf::Clock clock; // Zegar do deltaTime

    window.setFramerateLimit(60); // Ustawienie 60fps

    // Zmienne
    std::vector<Shooter> shooters;
    int money = 100;
    int moneyAdd = 15;

    //Fonty
    sf::Font mediumGothic("ScienceGothic-Medium.ttf");

    //Teksty
    //BOCZNY PANEL--------------------------
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
	sf::Texture sidePanel("sidePanel.png");

    //Sprite
	sf::Sprite sidePanelSprite(sidePanel);


    // Operacje wstêpne
    for (int i = 0; i < 10; i++)
    {
        shooters.emplace_back(jaguar1, randFloat(130.f, 170.f), randFloat(0.f, 688.f)); // generowanie shooterów, 688 aby nie wychodzi³ poza ekran 720-32
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
        }

        // Wyliczanie dt (czas miêdzy kolejnymi klatkami)
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Czyszczenie okna
        window.clear(sf::Color::White);

        //Boczne okienko


        //Update
        for (Shooter& shooter : shooters) {
            shooter.update(dt);
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

        for (Shooter& shooter : shooters) {
            shooter.draw(window);
        }

        window.display();
    }
}

float randFloat(float a, float b) {
    std::uniform_real_distribution<float> dist(a, b);

    return dist(rng);
}


