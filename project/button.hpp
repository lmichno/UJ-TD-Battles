#pragma once
#include <SFML/Graphics.hpp>

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
    Button(float x, float y, float width, float height, int costG)
        : isHovered(false), isPressed(false)
    {
        hitbox.setSize({ width, height });
        hitbox.setPosition({ x, y });
        hitbox.setFillColor(sf::Color::Transparent);

        overlay.setSize({ width, height });
        overlay.setPosition({ x, y });
        overlay.setFillColor(sf::Color::White);

		cost = costG;
    }

    // Aktualizacja stanu przycisku
    void update(sf::Vector2f mousePos, int money)
    {
        isHovered = hitbox.getGlobalBounds().contains(mousePos);

        if (isHovered && !isPressed && money - cost >= 0)
        {
            overlay.setFillColor(sf::Color(0, 255, 0, 50)); // Wysoka przezroczystoœæ przy najechaniu
        }
        else if (isPressed && money - cost >= 0)
        {
            overlay.setFillColor(sf::Color(0, 255, 0, 150)); // Niska przezroczystoœæ przy klikniêciu
        }
		else if (isHovered && !isPressed && money - cost < 0) 
        {
            overlay.setFillColor(sf::Color(255, 0, 0, 50)); // Czerwony przy najechaniu gdy brak kasy
        }
		else if (isPressed && money - cost < 0)
        {
            overlay.setFillColor(sf::Color(255, 0, 0, 150)); // Czerwony przy klikniêciu gdy brak kasy
        }
        else
        {
            overlay.setFillColor(sf::Color::Transparent);
        }
    }

    // Klikniêcie
    void onMousePressed()
    {
        if (isHovered)
        {
            isPressed = true;
        }
    }

    void onMouseReleased()
    {
        isPressed = false;
    }

    // Rysowanie
    void draw(sf::RenderWindow& window)
    {
        window.draw(overlay);
    }

    // Gettery
    bool getIsPressed() const
    {
        return isPressed;
    }

    bool getIsHovered() const
    {
        return isHovered;
    }

    //Obs³uga klikniêcia
    void onClicked()
    {
        // temp
    }
};
