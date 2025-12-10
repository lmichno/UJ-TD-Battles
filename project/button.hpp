#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
private:
    sf::RectangleShape hitbox;
    sf::RectangleShape overlay;
    bool isHovered;
    bool isPressed;
    int cost;

public:
    // Konstruktor
    Button(float x, float y, float width, float height, int costG);

    // Aktualizacja stanu przycisku
    void update(sf::Vector2f mousePos, int money);

    // Klikniêcie
    void onMousePressed();

    void onMouseReleased();

    // Rysowanie
    void draw(sf::RenderWindow& window);

    // Gettery
    bool getIsPressed() const;

    bool getIsHovered() const;

    //Obs³uga klikniêcia
    void onClicked(int &money);
};
