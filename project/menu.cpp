#include "menu.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <cstdint>
#include <algorithm>

GeneratorResult showGeneratorMenu(sf::RenderWindow& window)
{
    sf::Font font;
    font.openFromFile("LuckiestGuy-Regular.ttf");

    int selectedBg = 0;
    std::string moneyStr;
    std::string moneySecStr;
    std::string diffStr;
    int activeField = 0;

    while (window.isOpen())
    {

        sf::Vector2u win = window.getSize();
        float w = static_cast<float>(win.x);
        float h = static_cast<float>(win.y);

        sf::Vector2f mouse = window.mapPixelToCoords(
            sf::Mouse::getPosition(window),
            window.getView() 
        );

        //eventy
        while (const std::optional ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* m = ev->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f click = window.mapPixelToCoords(
                    sf::Vector2i{ m->position.x, m->position.y },
                    window.getView()
                );


            }
            else if (const auto* t = ev->getIf<sf::Event::TextEntered>())
            {
                if (activeField != 0)
                {
                    std::uint32_t code = t->unicode;

                    if (code == 8)
                    {
                        if (activeField == 1 && !moneyStr.empty()) moneyStr.pop_back();
                        if (activeField == 2 && !moneySecStr.empty()) moneySecStr.pop_back();
                        if (activeField == 3 && !diffStr.empty()) diffStr.pop_back();
                    }
                    else if (code >= '0' && code <= '9')
                    {
                        char c = static_cast<char>(code);
                        if (activeField == 1) moneyStr += c;
                        if (activeField == 2) moneySecStr += c;
                        if (activeField == 3) diffStr += c;
                    }
                    else if (code == '.' && activeField == 3)
                    {
                        diffStr += '.';
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        // napisy
        sf::Text title(font);
        title.setString("WYBIERZ TLO:");
        title.setCharacterSize(static_cast<unsigned int>(h * 0.06f));
        title.setFillColor(sf::Color::White);
        sf::FloatRect tb = title.getLocalBounds();
        title.setPosition(sf::Vector2f(
            w * 0.5f - tb.size.x * 0.5f,   // SFML 3: używamy size.x
            h * 0.05f
        ));
        window.draw(title);

        // tło
        float tileW = 150.0f, tileH = 80.0f;
        float startX = w * 0.5f - (tileW * 3.0f + 40.0f) * 0.5f;
        float tileY = h * 0.18f;

        std::vector<sf::RectangleShape> bgTiles;

        for (int i = 0; i < 3; ++i)
        {
            sf::RectangleShape r(sf::Vector2f(tileW, tileH));
            r.setPosition(sf::Vector2f(startX + i * (tileW + 20.0f), tileY));

            sf::Color col = sf::Color(120, 120, 120);
            if (selectedBg == i + 1)
                col = sf::Color::Green;
            else if (r.getGlobalBounds().contains(mouse))
                col = sf::Color(0, 120, 0);

            r.setFillColor(col);
            bgTiles.push_back(r);

            sf::Text t(font);
            t.setString(std::to_string(i + 1));
            t.setCharacterSize(static_cast<unsigned int>(tileH * 0.5f));
            t.setFillColor(sf::Color::White);
            t.setPosition(sf::Vector2f(
                r.getPosition().x + tileW * 0.4f,
                r.getPosition().y + tileH * 0.2f
            ));

            window.draw(r);
            window.draw(t);
        }

        // okienka do wpisania
        auto drawField = [&](const std::string& label,
            const std::string& value,
            float y,
            bool active,
            sf::RectangleShape& out)
            {
                sf::Text t(font);
                t.setString(label);
                t.setCharacterSize(static_cast<unsigned int>(h * 0.04f));
                t.setFillColor(sf::Color::White);
                t.setPosition(sf::Vector2f(w * 0.1f, y));
                window.draw(t);

                out = sf::RectangleShape(sf::Vector2f(w * 0.4f, h * 0.06f));
                out.setPosition(sf::Vector2f(w * 0.5f, y));
                out.setFillColor(active ? sf::Color(200, 200, 255) : sf::Color(180, 180, 180));
                window.draw(out);

                sf::Text val(font);
                val.setString(value);
                val.setCharacterSize(static_cast<unsigned int>(h * 0.04f));
                val.setFillColor(sf::Color::Black);
                val.setPosition(sf::Vector2f(out.getPosition().x + 10.0f,
                    out.getPosition().y + 5.0f));
                window.draw(val);
            };

        sf::RectangleShape box1, box2, box3;
        drawField("Ile gotowki?", moneyStr, h * 0.35f, activeField == 1, box1);
        drawField("Ile gotowki co sekunde?", moneySecStr, h * 0.47f, activeField == 2, box2);
        drawField("Wspolczynnik trudnosci:", diffStr, h * 0.59f, activeField == 3, box3);

        //ok
        sf::RectangleShape ok(sf::Vector2f(200.0f, 70.0f));
        ok.setPosition(sf::Vector2f(w * 0.3f, h * 0.75f));
        ok.setFillColor(ok.getGlobalBounds().contains(mouse) ? sf::Color(70, 170, 70) : sf::Color(100, 200, 100));

        sf::Text okTxt(font);
        okTxt.setString("OK");
        okTxt.setCharacterSize(40);
        okTxt.setFillColor(sf::Color::Black);
        okTxt.setPosition(sf::Vector2f(ok.getPosition().x + 70.0f, ok.getPosition().y + 10.0f));
        window.draw(ok);
        window.draw(okTxt);


        sf::RectangleShape exit(sf::Vector2f(200.0f, 70.0f));
        exit.setPosition(sf::Vector2f(w * 0.55f, h * 0.75f));
        exit.setFillColor(exit.getGlobalBounds().contains(mouse) ? sf::Color(170, 50, 50) : sf::Color(200, 80, 80));

        sf::Text exitTxt(font);
        exitTxt.setString("WYJDZ");
        exitTxt.setCharacterSize(40);
        exitTxt.setFillColor(sf::Color::Black);
        exitTxt.setPosition(sf::Vector2f(exit.getPosition().x + 40.0f, exit.getPosition().y + 10.0f));
        window.draw(exit);
        window.draw(exitTxt);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2f click = mouse;

            for (int i = 0; i < 3; ++i)
                if (bgTiles[i].getGlobalBounds().contains(click))
                    selectedBg = i + 1;

            if (box1.getGlobalBounds().contains(click)) activeField = 1;
            else if (box2.getGlobalBounds().contains(click)) activeField = 2;
            else if (box3.getGlobalBounds().contains(click)) activeField = 3;

            if (ok.getGlobalBounds().contains(click))
            {
                if (!moneyStr.empty() && !moneySecStr.empty() && !diffStr.empty() && selectedBg != 0)
                {
                    return GeneratorResult{
                        selectedBg,
                        std::stoi(moneyStr),
                        std::stoi(moneySecStr),
                        std::stof(diffStr)
                    };
                }
            }

            if (exit.getGlobalBounds().contains(click))
                return GeneratorResult{ 0, 0, 0, 0 };
        }

        window.display();
    }

    return GeneratorResult{ 0, 0, 0, 0 };
}

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
        //do generatora
        float genY = playY + tileH * 1.5f; // trochę niżej pod PLAY i EXIT
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Górny tytuł
        sf::Text title(font);
        title.setString("WYBIERZ POZIOM I TRUDNOSC:");
        title.setCharacterSize(static_cast<unsigned>(h * 0.06f));
        title.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = title.getLocalBounds();
            title.setPosition(sf::Vector2f(w * 0.5f - bounds.size.x * 0.5f,h * 0.08f));
        }

        // Tytuł 
        sf::Text difTitle(font);
        difTitle.setString("WYBIERZ POZIOM TRUDNOSCI:");
        difTitle.setCharacterSize(static_cast<unsigned>(h * 0.045f));
        difTitle.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = difTitle.getLocalBounds();
            difTitle.setPosition(sf::Vector2f( w * 0.5f - bounds.size.x * 0.5f,lvlY + tileH + 10.0f));
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
            txt.setPosition(sf::Vector2f(x + tileW * 0.25f,lvlY + tileH * 0.25f));
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
            txt.setPosition(sf::Vector2f(x + tileW * 0.25f,difY + tileH * 0.25f));
            diffTexts.push_back(txt);
        }

        // PLAY
        sf::RectangleShape play(sf::Vector2f(tileW * 1.5f, tileH));
        play.setPosition(sf::Vector2f(w * 0.5f - play.getSize().x * 1.2f,playY));
        play.setFillColor(sf::Color(200, 200, 50));
        if (play.getGlobalBounds().contains(mouse))
            play.setFillColor(sf::Color(180, 180, 30));

        sf::Text playTxt(font);
        playTxt.setString("PLAY");
        playTxt.setCharacterSize(static_cast<unsigned>(tileH * 0.45f));
        playTxt.setFillColor(sf::Color::Black);
        playTxt.setPosition(sf::Vector2f(
            play.getPosition().x + play.getSize().x * 0.3f,
            play.getPosition().y + tileH * 0.25f));

        //generator
        sf::RectangleShape genBtn({ tileW * 2.0f, tileH });
        genBtn.setPosition({ w * 0.5f - genBtn.getSize().x * 0.5f, genY });
        genBtn.setFillColor(genBtn.getGlobalBounds().contains(mouse) ? sf::Color(60, 130, 190) : sf::Color(80, 160, 220));

        sf::Text genTxt(font);
        genTxt.setString("GENERATOR");
        genTxt.setCharacterSize(tileH * 0.45f);
        genTxt.setFillColor(sf::Color::Black);
        genTxt.setPosition({
            genBtn.getPosition().x + genBtn.getSize().x * 0.2f,
            genBtn.getPosition().y + tileH * 0.25f
            });


        // EXIT
        sf::RectangleShape exitBtn(sf::Vector2f(tileW * 1.5f, tileH));
        exitBtn.setPosition(sf::Vector2f(
            w * 0.5f + play.getSize().x * 0.2f,playY ));
        exitBtn.setFillColor(sf::Color(200, 80, 80));
        if (exitBtn.getGlobalBounds().contains(mouse))
            exitBtn.setFillColor(sf::Color(170, 50, 50));

        sf::Text exitTxt(font);
        exitTxt.setString("EXIT");
        exitTxt.setCharacterSize(static_cast<unsigned>(tileH * 0.45f));
        exitTxt.setFillColor(sf::Color::Black);
        exitTxt.setPosition(sf::Vector2f(
        exitBtn.getPosition().x + exitBtn.getSize().x * 0.3f, exitBtn.getPosition().y + tileH * 0.25f));


        // EVENTY
        while (const std::optional ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
                window.close();

            if (const auto* m = ev->getIf<sf::Event::MouseButtonPressed>())
            {

                sf::Vector2f click = window.mapPixelToCoords( sf::Vector2i(m->position.x, m->position.y) );
                if (exitBtn.getGlobalBounds().contains(click))
                {
                    window.close();
                }

                for (int i = 0; i < 3; ++i)
                {
                    if (lvlButtons[i].getGlobalBounds().contains(click))
                        lvl = i + 1;

                    if (diffButtons[i].getGlobalBounds().contains(click))
                        difficulty = i + 1;
                }
                if (exitBtn.getGlobalBounds().contains(click))
                {
                    window.close();
                }

                // --- GENERATOR ---
                if (genBtn.getGlobalBounds().contains(click))
                {
                    GeneratorResult gen = showGeneratorMenu(window);

                    if (gen.background != 0) 
                    {
                        if (gen.background != 0)
                        {
                            return MenuResult{
                                gen.background,
                                static_cast<int>(gen.difficulty),
                                gen.money2,
                                gen.moneyPerSec,
                                gen.difficulty,
                                false
                            };
                        }

                    }
                }

                // --- WYBÓR LVL ---
                for (int i = 0; i < 3; ++i)
                {
                    if (lvlButtons[i].getGlobalBounds().contains(click))
                        lvl = i + 1;
                }

                // --- WYBÓR TRUDNOŚCI ---
                for (int i = 0; i < 3; ++i)
                {
                    if (diffButtons[i].getGlobalBounds().contains(click))
                        difficulty = i + 1;
                }

                // --- PLAY ---
                if (play.getGlobalBounds().contains(click) && lvl != 0 && difficulty != 0)
                {
                    return MenuResult{ lvl, difficulty };
                }


                if (play.getGlobalBounds().contains(click) &&
                    lvl != 0 && difficulty != 0)
                {
                    return MenuResult{ lvl, difficulty };
                }
            }
        }

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
        window.draw(genBtn);
        window.draw(genTxt);
        window.draw(exitBtn);
        window.draw(exitTxt);
        window.display();
    }

    return MenuResult{ lvl, difficulty };
}
