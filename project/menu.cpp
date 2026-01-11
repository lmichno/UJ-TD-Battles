#include "menu.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <algorithm>

MenuResult showMenu(sf::RenderWindow& window)
{
    sf::Font font("LuckiestGuy-Regular.ttf");

    int lvl = 0;
    int difficulty = 0;

    while (window.isOpen())
    {
        sf::Vector2u win = window.getSize();
        float w = static_cast<float>(win.x);
        float h = static_cast<float>(win.y);

        // Skalowanie i centrowanie kafelków w rzędzie
        float maxRowWidth = w * 0.9f;
        float spacingRatio = 0.35f;

        float tileW = std::min(maxRowWidth / (3.0f + 2.0f * spacingRatio), 220.0f);
        float spacing = tileW * spacingRatio;
        float tileH = std::min(h * 0.12f, 100.0f);

        float rowWidth = tileW * 3.0f + spacing * 2.0f;
        float startX = (w - rowWidth) * 0.5f;

        float lvlY = h * 0.25f;
        float difY = h * 0.48f;   // dolne kafelki niżej
        float playY = h * 0.70f;

        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Górny tytuł
        sf::Text title(font);
        title.setString("WYBIERZ POZIOM I TRUDNOSC:");
        title.setCharacterSize(static_cast<unsigned>(h * 0.06f));
        title.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = title.getLocalBounds();
            title.setPosition(sf::Vector2f(
                w * 0.5f - bounds.size.x * 0.5f,
                h * 0.08f
            ));
        }

        // Tytuł nad DIF, wyrównany z górnym
        sf::Text difTitle(font);
        difTitle.setString("WYBIERZ POZIOM TRUDNOSCI:");
        difTitle.setCharacterSize(static_cast<unsigned>(h * 0.045f));
        difTitle.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = difTitle.getLocalBounds();
            difTitle.setPosition(sf::Vector2f(
                w * 0.5f - bounds.size.x * 0.5f,
                lvlY + tileH + 10.0f
            ));
        }

        std::vector<sf::RectangleShape> lvlButtons;
        std::vector<sf::Text>           lvlTexts;
        std::vector<sf::RectangleShape> diffButtons;
        std::vector<sf::Text>           diffTexts;

        // Kafle LVL
        for (int i = 0; i < 3; ++i)
        {
            float x = startX + i * (tileW + spacing);

            sf::RectangleShape rect(sf::Vector2f(tileW, tileH));
            rect.setPosition(sf::Vector2f(x, lvlY));

            sf::Color col(150, 150, 150);
            if (lvl == i + 1)
                col = sf::Color::Green;
            if (rect.getGlobalBounds().contains(mouse))
                col = sf::Color(0, 120, 0);
            rect.setFillColor(col);

            lvlButtons.push_back(rect);

            sf::Text txt(font);
            txt.setString("LVL " + std::to_string(i + 1));
            txt.setCharacterSize(static_cast<unsigned>(tileH * 0.4f));
            txt.setFillColor(sf::Color::White);
            txt.setPosition(sf::Vector2f(
                x + tileW * 0.25f,
                lvlY + tileH * 0.25f
            ));

            lvlTexts.push_back(txt);
        }

        // Kafle DIF
        for (int i = 0; i < 3; ++i)
        {
            float x = startX + i * (tileW + spacing);

            sf::RectangleShape rect(sf::Vector2f(tileW, tileH));
            rect.setPosition(sf::Vector2f(x, difY));

            sf::Color col(150, 150, 150);
            if (difficulty == i + 1)
                col = sf::Color::Green;
            if (rect.getGlobalBounds().contains(mouse))
                col = sf::Color(0, 120, 0);
            rect.setFillColor(col);

            diffButtons.push_back(rect);

            sf::Text txt(font);
            txt.setString("DIF " + std::to_string(i + 1));
            txt.setCharacterSize(static_cast<unsigned>(tileH * 0.4f));
            txt.setFillColor(sf::Color::White);
            txt.setPosition(sf::Vector2f(
                x + tileW * 0.25f,
                difY + tileH * 0.25f
            ));

            diffTexts.push_back(txt);
        }

        // Przycisk PLAY
        sf::RectangleShape play(sf::Vector2f(tileW * 1.5f, tileH));
        play.setPosition(sf::Vector2f(
            w * 0.5f - play.getSize().x * 0.5f,
            playY
        ));
        play.setFillColor(sf::Color(200, 200, 50));
        if (play.getGlobalBounds().contains(mouse))
            play.setFillColor(sf::Color(180, 180, 30));

        sf::Text playTxt(font);
        playTxt.setString("PLAY");
        playTxt.setCharacterSize(static_cast<unsigned>(tileH * 0.45f));
        playTxt.setFillColor(sf::Color::Black);
        playTxt.setPosition(sf::Vector2f(
            play.getPosition().x + play.getSize().x * 0.3f,
            play.getPosition().y + tileH * 0.25f
        ));

        // EVENTY
        while (const std::optional ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
                window.close();

            if (const auto* m = ev->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f click = window.mapPixelToCoords(
                    sf::Vector2i(m->position.x, m->position.y)
                );

                for (int i = 0; i < 3; ++i)
                {
                    if (lvlButtons[i].getGlobalBounds().contains(click))
                        lvl = i + 1;

                    if (diffButtons[i].getGlobalBounds().contains(click))
                        difficulty = i + 1;
                }

                if (play.getGlobalBounds().contains(click) &&
                    lvl != 0 && difficulty != 0)
                {
                    return MenuResult{ lvl, difficulty };
                }
            }
        }

        // RYSOWANIE
        window.clear(sf::Color::Black);
        window.draw(title);
        window.draw(difTitle);

        for (int i = 0; i < 3; ++i)
        {
            window.draw(lvlButtons[i]);
            window.draw(lvlTexts[i]);
            window.draw(diffButtons[i]);
            window.draw(diffTexts[i]);
        }

        window.draw(play);
        window.draw(playTxt);
        window.display();
    }

    return MenuResult{ lvl, difficulty };
}
