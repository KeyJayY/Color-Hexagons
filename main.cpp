#include <iostream>
#include <numeric>

#include "SFML/Graphics.hpp"
#include "ShareTechMono-Regular.h"

#include "hexagons.h"
#include "slider.h"

std::shared_ptr<sf::Font> font;

int WinMain(){
    sf::RenderWindow window(sf::VideoMode(1000, 800), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

    bool clicked = false;
    float value = 1;
    hexagon_RGB h_RGB(sf::Vector2f(0, 0), sf::Vector2f(160, 160), "RGB");
    hexagon_CMY h_CMY(sf::Vector2f(170, 0), sf::Vector2f(330, 160), "CMY");
    hexagon_HSL h_HSL(sf::Vector2f(0, 170), sf::Vector2f(160, 330), "HSL");
    hexagon_HSB h_HSB(sf::Vector2f(170, 170), sf::Vector2f(330, 330), "HSB");
    Slider slider;

    sf::Vector2f canvasSize(window.getSize().x - 160, window.getSize().y - 20);
    h_RGB.resize(sf::Vector2f(0, 0), sf::Vector2f((canvasSize.x) / 2 - 10, window.getSize().y / 2 - 10));
    h_CMY.resize(sf::Vector2f((canvasSize.x) / 2 + 10, 0), sf::Vector2f((canvasSize.x), canvasSize.y / 2 - 10));
    h_HSL.resize(sf::Vector2f(0, canvasSize.y / 2 + 10), sf::Vector2f((canvasSize.x) / 2 - 10, canvasSize.y));
    h_HSB.resize(sf::Vector2f((canvasSize.x) / 2 + 10, canvasSize.y / 2 + 10), sf::Vector2f((canvasSize.x), canvasSize.y));
    slider.setPosition(window.getSize());

    std::string cords;
    sf::Clock frame_clock, around_half_secound_clock;
    sf::Int64 mean_frames_time = 0;
    std::vector<sf::Int64> frame_times;
    sf::Text text;

    font = std::make_shared<sf::Font>();
    font->loadFromMemory(&(ShareTechMono_Regular_ttf[0]),
        ShareTechMono_Regular_ttf.size());
    text.setFont(*font);
    text.setCharacterSize(21);
    text.setFillColor(sf::Color::Black);
    text.setPosition(window.getSize().x - 120, window.getSize().y - 150);

    around_half_secound_clock.restart();

    while (window.isOpen()) {
        sf::Event event;
        window.clear(sf::Color::White);
        text.setPosition(window.getSize().x - 120, window.getSize().y - 150);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    value = slider.mouseEvent(mousePosition);
                    clicked = true;
                    h_RGB.changeLight(value);
                    h_CMY.changeLight(value);
                    h_HSL.changeLight(value);
                    h_HSB.changeLight(value);
                    if (h_RGB.click(mousePosition) != "") {
                        cords = h_RGB.click(mousePosition);
                    }
                    else if (h_CMY.click(mousePosition) != "") {
                        cords = h_CMY.click(mousePosition);
                    }
                    else if (h_HSL.click(mousePosition) != "") {
                        cords = h_HSL.click(mousePosition);
                    }
                    else if (h_HSB.click(mousePosition) != "") {
                        cords = h_HSB.click(mousePosition);
                    }
                    else {
                        cords = "";
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    clicked = false;
                }
            }
            if (event.type == sf::Event::MouseMoved && clicked) {
                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                value = slider.mouseEvent(mousePosition);
                h_RGB.changeLight(value);
                h_CMY.changeLight(value);
                h_HSL.changeLight(value);
                h_HSB.changeLight(value);
            }
            if (event.type == sf::Event::Resized) {
                if (window.getSize().x < 700) {
                    window.setSize(sf::Vector2u(700, window.getSize().y));
                }
                if (window.getSize().y < 600) {
                    window.setSize(sf::Vector2u(window.getSize().x, 600));
                }
                window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
                sf::Vector2f canvasSize(window.getSize().x - 160, window.getSize().y - 20);
                h_RGB.resize(sf::Vector2f(0, 0), sf::Vector2f((canvasSize.x) / 2 - 10, window.getSize().y / 2 - 10));
                h_CMY.resize(sf::Vector2f((canvasSize.x) / 2 + 10, 0), sf::Vector2f((canvasSize.x), canvasSize.y / 2 - 10));
                h_HSL.resize(sf::Vector2f(0, canvasSize.y / 2 + 10), sf::Vector2f((canvasSize.x) / 2 - 10, canvasSize.y));
                h_HSB.resize(sf::Vector2f((canvasSize.x) / 2 + 10, canvasSize.y / 2 + 10), sf::Vector2f((canvasSize.x), canvasSize.y));
                slider.setPosition(window.getSize());
                text.setPosition(window.getSize().x - 120, window.getSize().y - 150);
            }
        }

        window.draw(h_RGB);
        window.draw(h_CMY);
        window.draw(h_HSL);
        window.draw(h_HSB);
        window.draw(slider);

        text.setString(std::to_string(mean_frames_time) + "us");
        window.draw(text);
        text.setString(cords);
        text.setPosition(window.getSize().x - 225, window.getSize().y - 40);
        window.draw(text);


        frame_times.push_back(frame_clock.getElapsedTime().asMicroseconds());
        if (around_half_secound_clock.getElapsedTime().asSeconds() >= 0.5f &&
            frame_times.size() >= 1) {
            mean_frames_time = (sf::Int64)(
                (float)std::reduce(frame_times.begin(), frame_times.end()) /
                (float)frame_times.size());
            frame_times.clear();
            around_half_secound_clock.restart();
        }

        frame_clock.restart();
        window.display();
    }

    font.reset();

    return 0;
}