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

    //Fonty
    sf::Font mediumGothic("ScienceGothic-Medium.ttf");

    //Teksty
    sf::Text text(mediumGothic);
    text.setString("UJ TD battles");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Black);
    
    // Tekstury
    sf::Texture jaguar1("jaguar1.png", false, sf::IntRect({ 0, 0 }, { 32, 64 }));

    std::vector<Shooter> shooters;

    for (int i = 0; i < 10; i++)
    {
        shooters.emplace_back(jaguar1, randFloat(10.f, 50.f), randFloat(0.f, 688.f)); // generowanie shooterów, 688 aby nie wychodzi³ poza ekran 720-32
    }


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


        window.clear(sf::Color::White);

        //Temp::::
        sf::RectangleShape rectangle({5, 720});
        rectangle.setFillColor(sf::Color::Black);
        rectangle.setPosition({70, 0});
        //Temp::::

        //Update
        for (Shooter& shooter : shooters) {
            shooter.update(dt);
        }

        // Draw
        window.draw(rectangle);
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


