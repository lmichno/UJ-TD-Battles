#pragma once
#include <SFML/Graphics.hpp>

struct MenuResult
{
    int lvl;
    int difficulty;

    int money2;
    int moneyPerSec;
    float genDifficulty;

    bool fromGenerator; // ← TO JEST TO, CZEGO BRAKOWAŁO
};
struct GeneratorResult{
    int background;
    int money2;
    int moneyPerSec;
    float difficulty;
};
MenuResult showMenu(sf::RenderWindow& window);
GeneratorResult showGeneratorMenu(sf::RenderWindow& window);

