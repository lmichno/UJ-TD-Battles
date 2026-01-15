#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>


//ustawienia muzyki
void musicS(sf::RenderWindow& window)
{
    static sf::Music music;

    if (!music.openFromFile("music.ogg"))
        return;
    music.setLooping(true);
    music.play();

    sf::Font font("ScienceGothic-Medium.ttf");

    sf::Text onText(font);
    onText.setString("ON");
    onText.setCharacterSize(32);
    onText.setFillColor(sf::Color::Green);
    onText.setPosition({ 560.f, 200.f });

    sf::Text offText(font);
    offText.setString("OFF");
    offText.setCharacterSize(32);
    offText.setFillColor(sf::Color::White);
    offText.setPosition({ 560.f, 260.f });

    sf::Text okText(font);
    okText.setString("OK");
    okText.setCharacterSize(32);
    okText.setFillColor(sf::Color::White);
    okText.setPosition({ 610.f, 410.f });

    sf::FloatRect onRect({ 560.f, 200.f }, { 160.f, 50.f });
    sf::FloatRect offRect({ 560.f, 260.f }, { 160.f, 50.f });

    float volume = 50.f;
    music.setVolume(volume);

    sf::RectangleShape sliderBar({ 300.f, 5.f });
    sliderBar.setFillColor(sf::Color::White);
    sliderBar.setPosition({ 490.f, 330.f });

    sf::RectangleShape sliderKnob({ 20.f, 40.f });
    sliderKnob.setFillColor(sf::Color::Red);
    sliderKnob.setPosition({ 490.f + volume * 3.f, 315.f });

    sf::RectangleShape okButton({ 200.f, 60.f });
    okButton.setFillColor(sf::Color(70, 70, 70));
    okButton.setPosition({ 540.f, 400.f });

    bool dragging = false;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                return;

            sf::Vector2i mp = sf::Mouse::getPosition(window);
            sf::Vector2f mouse(static_cast<float>(mp.x), static_cast<float>(mp.y));

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (onRect.contains(mouse))
                {
                    music.play();
                    onText.setFillColor(sf::Color::Green);
                    offText.setFillColor(sf::Color::White);
                }

                if (offRect.contains(mouse))
                {
                    music.pause();
                    offText.setFillColor(sf::Color::Red);
                    onText.setFillColor(sf::Color::White);
                }


                if (sliderKnob.getGlobalBounds().contains(mouse))
                    dragging = true;

                if (okButton.getGlobalBounds().contains(mouse))
                    return;
            }

            if (event->is<sf::Event::MouseButtonReleased>())
                dragging = false;
        }

        if (dragging)
        {
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            float x = std::clamp(static_cast<float>(mp.x), 490.f, 790.f);
            sliderKnob.setPosition({ x, 315.f });

            volume = (x - 490.f) / 3.f;
            music.setVolume(volume);
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(onText);
        window.draw(offText);
        window.draw(sliderBar);
        window.draw(sliderKnob);
        window.draw(okButton);
        window.draw(okText);
        window.display();
    }
}

//g³ówne menmu

bool TVMENU(sf::RenderWindow& window)
{
    sf::Font font("ScienceGothic-Medium.ttf");

    sf::Text settingsText(font);
    settingsText.setString("USTAWIENIA");
    settingsText.setCharacterSize(40);
    settingsText.setFillColor(sf::Color::White);
    settingsText.setPosition({ 520.f, 250.f });

    sf::Text startText(font);
    startText.setString("START");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({ 560.f, 330.f });

    sf::FloatRect settingsRect({ 520.f, 250.f }, { 240.f, 60.f });
    sf::FloatRect startRect({ 560.f, 330.f }, { 160.f, 60.f });

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                return false;

            sf::Vector2i mp = sf::Mouse::getPosition(window);
            sf::Vector2f mouse(static_cast<float>(mp.x), static_cast<float>(mp.y));

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                if (settingsRect.contains(mouse))
                    musicS(window);

                if (startRect.contains(mouse))
                    return true;
            }
        }

        window.clear(sf::Color(20, 20, 20));
        window.draw(settingsText);
        window.draw(startText);
        window.display();
    }

    return false;
}
