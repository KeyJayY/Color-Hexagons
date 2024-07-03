#pragma once
#include "SFML/Graphics.hpp"

class Indicator : public sf::Drawable {
private:
    sf::ConvexShape triangle1, triangle2;
    float a = 30;
    float width = 100;
    std::string name;
public:
    Indicator() {
        triangle1.setPointCount(3);
        triangle2.setPointCount(3);
        triangle1.setFillColor(sf::Color::Black);
        triangle2.setFillColor(sf::Color::Black);
        triangle1.setPoint(0, sf::Vector2f(0, 0));
        triangle1.setPoint(1, sf::Vector2f(0, a));
        triangle1.setPoint(2, sf::Vector2f(a / 2 * 1.73, a / 2));

        triangle2.setPoint(0, sf::Vector2f(width + a / 2 * 1.73, 0));
        triangle2.setPoint(1, sf::Vector2f(width + a / 2 * 1.73, a));
        triangle2.setPoint(2, sf::Vector2f(width, a / 2));
    }
    void setPosition(sf::Vector2f rectPosition, sf::Vector2f rectSize, float value) {
        triangle1.setPosition(rectPosition.x - a / 2 * 1.7, rectPosition.y + rectSize.y * value - a/2);
        triangle2.setPosition(rectPosition.x, rectPosition.y + rectSize.y * value - a / 2);
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(triangle1);
        target.draw(triangle2);
    }
};

class Slider : public sf::Drawable {
private:
	int width = 100;
	int height = 300;
    float value = 1;
    sf::Texture texture;
    sf::Vector2f position;
    sf::RectangleShape rectangle;
    Indicator indicator;
public:
	Slider() {
        texture.create(400, 300);
        sf::Uint8* pixels = new sf::Uint8[400 * 300 * 4];
        for (int y = 0; y < 300; ++y) {
            for (int x = 0; x < 400; ++x) {
                float ratio = static_cast<float>(y) / 300;
                sf::Uint8 r = static_cast<sf::Uint8>(255 * ratio);
                sf::Uint8 g = static_cast<sf::Uint8>(255 * ratio);
                sf::Uint8 b = static_cast<sf::Uint8>(255 * ratio);

                pixels[(y * 400 + x) * 4 + 0] = r;
                pixels[(y * 400 + x) * 4 + 1] = g;
                pixels[(y * 400 + x) * 4 + 2] = b;
                pixels[(y * 400 + x) * 4 + 3] = 255; 
            }
        }
        texture.update(pixels);
        rectangle.setTexture(&texture);
        rectangle.setSize(sf::Vector2f(width, height));
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setOutlineThickness(2);
	}
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(rectangle, states);
        target.draw(indicator);
    }

    void setPosition(sf::Vector2u window) {
        rectangle.setSize(sf::Vector2f(width, window.y - 200));
        position.x = window.x - width - 40;
        position.y = 20;
        rectangle.setPosition(position);
        indicator.setPosition(rectangle.getPosition(), rectangle.getSize(), value);
    }
    float mouseEvent(sf::Vector2f mousePosition) {
        if (rectangle.getGlobalBounds().contains(mousePosition)) {
            value = (mousePosition.y - rectangle.getPosition().y) / rectangle.getSize().y;
            if (value <= 0) value = 0.0001;
            if (value > 1) value = 1;
            indicator.setPosition(rectangle.getPosition(), rectangle.getSize(), value);
        }
        return value;
    }
};