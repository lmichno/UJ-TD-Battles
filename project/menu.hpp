#pragma once
#include <SFML/Graphics.hpp>

struct MenuResult {
    int lvl;
    int difficulty;
};

MenuResult showMenu(sf::RenderWindow& window);
