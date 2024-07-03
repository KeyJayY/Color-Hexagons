#pragma once

#include <iostream>
#include <numeric>

#include "Roboto.h"
#include "SFML/Graphics.hpp"
#define SQRT3 1.71605080757

class hexagon : public sf::Drawable {
   protected:
    float a1 = 1, b1 = 1, c1 = 1;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;
    float radius;
    sf::ConvexShape hexagon1;
    sf::Vector2f left_top, right_bottom, center;
    sf::Vector2f p[6];
    std::string name;
    float* pixels;
    bool configure(float& a, float& b, float& c, sf::Vector2f point) const;
    virtual void convertToRGB(float&, float&, float&) const = 0;
    void setAngles();
    void setTexture();
    inline void fillPixels(sf::Uint8* pixels2, int y, int x, int a, int b,
                           int c);
    void setFont();

   public:
    std::string click(sf::Vector2f mousePosition);
    void setPixels();
    void resize(sf::Vector2f newLeftTop, sf::Vector2f newRightBottom);
    void Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom);
    void Draw_Border(sf::RenderTarget& target, sf::RenderStates states,
                     sf::String name) const;
    bool rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float& alpha,
                 float& beta) const;
    void drawText(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void changeLight(float value) = 0;
    hexagon(sf::Vector2f left, sf::Vector2f right, std::string name)
        : left_top(left), right_bottom(right), name(name) {
        setPixels();
        hexagon1.setOutlineColor(sf::Color::Black);
        hexagon1.setOutlineThickness(5);
        radius = std::abs(left_top.x - right_bottom.x) / 2 - 20;
        center.x = (left_top.x + right_bottom.x) / 2;
        center.y = (left_top.y + right_bottom.y) / 2;
        hexagon1.setPointCount(6);
        // setAngles();
        hexagon1.setFillColor(sf::Color::Transparent);
        Set_Borders(left_top, right_bottom);
        setFont();
    };

   private:
    float d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const;
};

std::string hexagon::click(sf::Vector2f mousePosition) {
    float a, b, c;
    std::string txt;
    if (configure(a, b, c, sf::Vector2f(mousePosition.x, mousePosition.y))) {
        int ia = a * a1 * 255, ib = b * b1 * 255, ic = c * c1 * 255;
        txt = name + " (" + std::to_string(ia) + ", " + std::to_string(ib) +
              ", " + std::to_string(ic) + ")";
    } else {
        txt = "";
    }
    return txt;
}

void hexagon::drawText(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(p[0].x, p[0].y - 25);
    text.setString(name[0]);
    target.draw(text);
    text.setPosition(p[2].x - 25, p[2].y - 15);
    text.setString(name[1]);
    target.draw(text);
    text.setPosition(p[4].x + 7, p[4].y - 15);
    text.setString(name[2]);
    target.draw(text);
    text.setPosition(left_top.x + 25, left_top.y + 25);
    text.setString(name);
    target.draw(text);
}

void hexagon::setFont() {
    font.loadFromMemory(Roboto_Regular_ttf, Roboto_Regular_ttf_len);
}

void hexagon::resize(sf::Vector2f newLeftTop, sf::Vector2f newRightBottom) {
    left_top = newLeftTop;
    right_bottom = newRightBottom;
    radius = std::abs(left_top.x - right_bottom.x) / 2 - 20;
    center.x = (left_top.x + right_bottom.x) / 2;
    center.y = (left_top.y + right_bottom.y) / 2;
    Set_Borders(left_top, right_bottom);
    // setAngles();
    sprite.setPosition(p[0]);
    sprite.setScale((p[3].y - p[0].y) / 160, (p[3].y - p[0].y) / 160);
    setTexture();
    hexagon1.setPoint(0, sf::Vector2f(p[0].x, p[0].y + 5));
    hexagon1.setPoint(1, sf::Vector2f(p[1].x + 2, p[1].y + 4));
    hexagon1.setPoint(2, sf::Vector2f(p[2].x + 2, p[2].y - 2));
    hexagon1.setPoint(3, sf::Vector2f(p[3].x, p[3].y - 3));
    hexagon1.setPoint(4, sf::Vector2f(p[4].x - 2, p[4].y - 2));
    hexagon1.setPoint(5, sf::Vector2f(p[5].x - 2, p[5].y + 4));
}

void hexagon::setPixels() {
    float a, b, c;
    pixels = new float[160 * 160 * 4];
    sf::Vector2f left_top(0, 0);
    p[0] = sf::Vector2f(80, 0);
    p[1] = sf::Vector2f(10.718, 40);
    p[2] = sf::Vector2f(10.718, 120);
    p[3] = sf::Vector2f(80, 160);
    p[4] = sf::Vector2f(149.282, 120);
    p[5] = sf::Vector2f(149.282, 40);
    for (int y = 0; y < 160; y++) {
        for (int x = 0; x < 160; x++) {
            if (configure(a, b, c,
                          sf::Vector2f(x + left_top.x, y + left_top.y))) {
                pixels[(y * 160 + x) * 4 + 0] = a;
                pixels[(y * 160 + x) * 4 + 1] = b;
                pixels[(y * 160 + x) * 4 + 2] = c;
                pixels[(y * 160 + x) * 4 + 3] = 255;
            } else {
                pixels[(y * 160 + x) * 4 + 0] = 0;
                pixels[(y * 160 + x) * 4 + 1] = 0;
                pixels[(y * 160 + x) * 4 + 2] = 0;
                pixels[(y * 160 + x) * 4 + 3] = 0;
            }
        }
    }
}

bool hexagon::configure(float& a, float& b, float& c,
                        sf::Vector2f point) const {
    float alpha, beta;
    float ar, br, cr;
    ar = rhombus(p[0], p[1], point, alpha, beta);
    a = ar ? 1 : 2 - std::max(alpha, beta);
    br = rhombus(p[2], p[3], point, alpha, beta);
    b = br ? 1 : 2 - std::max(alpha, beta);
    cr = rhombus(p[4], p[5], point, alpha, beta);
    c = cr ? 1 : 2 - std::max(alpha, beta);
    return ar || br || cr;
}

float hexagon::d_coefficient(sf::Vector2f p0, sf::Vector2f p1,
                             sf::Vector2f p) const {
    float A = p0.y - p1.y;
    float B = p1.x - p0.x;
    float C = p0.x * (p1.y - p0.y) - p0.y * (p1.x - p0.x);

    return 2 * ((A * p.x + B * p.y + C) / sqrt(A * A + B * B)) / sqrt(3.0f);
}

bool hexagon::rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p,
                      float& alpha, float& beta) const {
    float L =
        sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
    sf::Vector2f p3(
        0.5f * (p0.x - p1.x) + 0.5f * sqrt(3.0f) * (p1.y - p0.y) + p0.x,
        0.5f * sqrt(3.0f) * (p0.x - p1.x) - 0.5f * (p1.y - p0.y) + p0.y);

    beta = -d_coefficient(p0, p1, p) / L;
    alpha = d_coefficient(p0, p3, p) / L;

    if ((alpha > 1.0f) || (beta > 1.0f) || (alpha < 0.0f) || (beta < 0.0f))
        return false;
    else
        return true;
}

void hexagon::Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom) {
    left_top = _left_top;
    right_bottom = _right_bottom;
    float a = ((right_bottom.y - left_top.y) / 2.0f + 0.5f) - 20.0f;
    center = left_top +
             sf::Vector2f((right_bottom.x - left_top.x) / 2.0f + 0.5f,
                          (right_bottom.y - left_top.y) / 2.0f + 0.5f + 10);

    p[0] = center - sf::Vector2f(0.0f, a);
    p[1] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[2] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
    p[3] = center + sf::Vector2f(0.0f, a);
    p[4] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[5] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
}

void hexagon::Draw_Border(sf::RenderTarget& target, sf::RenderStates states,
                          sf::String name) const {
    target.draw(hexagon1, states);
}

void hexagon::setAngles() {
    for (int i = 0; i < 6; ++i) {
        float angle = (60 * (-i) - 90) * 3.14159265f / 180.f;
        p[i].x = center.x + cos(angle) * radius;
        p[i].y = center.y + sin(angle) * radius;
        hexagon1.setPoint(i, p[i]);
    }
}

class hexagon_RGB : public hexagon {
   public:
    void changeLight(float light) override;
    void convertToRGB(float& r, float& g, float& b) const override;
    hexagon_RGB(sf::Vector2f left, sf::Vector2f right, std::string name)
        : hexagon(left, right, name) {
        setTexture();
    };
};

inline void hexagon::fillPixels(sf::Uint8* pixels2, int y, int x, int a, int b,
                                int c) {
    if (pixels[(y * 160 + x) * 4 + 3] != 0) {
        pixels2[(y * 160 + x) * 4 + 0] = a;
        pixels2[(y * 160 + x) * 4 + 1] = b;
        pixels2[(y * 160 + x) * 4 + 2] = c;
        pixels2[(y * 160 + x) * 4 + 3] = 255;
    } else {
        pixels2[(y * 160 + x) * 4 + 0] = 255;
        pixels2[(y * 160 + x) * 4 + 1] = 255;
        pixels2[(y * 160 + x) * 4 + 2] = 255;
        pixels2[(y * 160 + x) * 4 + 3] = 0;
    }
}

void hexagon::setTexture() {
    texture.create(160, 160);
    float a, b, c;
    sf::Uint8* pixels2 = new sf::Uint8[160 * 160 * 4];
    for (int y = 0; y < 160; y++) {
        for (int x = 0; x < 160; x++) {
            a = pixels[(y * 160 + x) * 4 + 0] * a1;
            b = pixels[(y * 160 + x) * 4 + 1] * b1;
            c = pixels[(y * 160 + x) * 4 + 2] * c1;
            convertToRGB(a, b, c);
            fillPixels(pixels2, y, x, a, b, c);
        }
    }
    texture.update(pixels2);
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setOrigin(80, 0);
    sprite.setPosition(p[0]);
    delete[] pixels2;
}

void hexagon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
    Draw_Border(target, states, name);
    drawText(target, states);
}

void hexagon_RGB::convertToRGB(float& red, float& green, float& blue) const {
    red *= 255;
    green *= 255;
    blue *= 255;
}

class hexagon_CMY : public hexagon {
   public:
    void changeLight(float light) override;
    void convertToRGB(float& cyan, float& magenta,
                      float& yellow) const override;
    hexagon_CMY(sf::Vector2f left, sf::Vector2f right, std::string name)
        : hexagon(left, right, name) {
        setTexture();
        // sprite.setScale(3.f, 3.f);
    };
};

void hexagon_CMY::convertToRGB(float& cyan, float& magenta,
                               float& yellow) const {
    cyan = 255 - (cyan * 255);
    magenta = 255 - (magenta * 255);
    yellow = 255 - (yellow * 255);
}

class hexagon_HSL : public hexagon {
   public:
    void changeLight(float light) override;
    void convertToRGB(float& hue, float& saturation,
                      float& lightness) const override;
    hexagon_HSL(sf::Vector2f left, sf::Vector2f right, std::string name)
        : hexagon(left, right, name) {
        setTexture();
        // sprite.setScale(3.f, 3.f);
    };

   private:
    float HueToRGB(float v1, float v2, float vH) const;
};

float hexagon_HSL::HueToRGB(float v1, float v2, float vH) const {
    if (vH < 0) vH += 1;

    if (vH > 1) vH -= 1;

    if ((6 * vH) < 1) return (v1 + (v2 - v1) * 6 * vH);

    if ((2 * vH) < 1) return v2;

    if ((3 * vH) < 2) return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

    return v1;
}

void hexagon_HSL::convertToRGB(float& h, float& s, float& l) const {
    float r = 0;
    float g = 0;
    float b = 0;

    if (s == 0) {
        r = g = b = (unsigned char)(l * 255);
    } else {
        float v1, v2;

        v2 = (l < 0.5) ? (l * (1 + s)) : ((l + s) - (l * s));
        v1 = 2 * l - v2;

        r = (255 * HueToRGB(v1, v2, h + (1.0f / 3)));
        g = (255 * HueToRGB(v1, v2, h));
        b = (255 * HueToRGB(v1, v2, h - (1.0f / 3)));
    }
    h = r;
    s = g;
    l = b;
}

class hexagon_HSB : public hexagon {
   public:
    void changeLight(float light) override;
    void convertToRGB(float& hue, float& saturation,
                      float& brightness) const override;
    hexagon_HSB(sf::Vector2f left, sf::Vector2f right, std::string name)
        : hexagon(left, right, name) {
        setTexture();
        // sprite.setScale(3.f, 3.f);
    };
};

void hexagon_HSB::convertToRGB(float& hue, float& saturation,
                               float& brightness) const {
    hue = fmod(hue, 1.0f);
    float chroma = brightness * saturation;
    float hue_segment = hue * 6.0f;
    float x = chroma * (1 - std::abs(fmod(hue_segment, 2) - 1));
    float r1, g1, b1;
    float r, g, b;

    if (hue_segment >= 0 && hue_segment < 1) {
        r1 = chroma;
        g1 = x;
        b1 = 0;
    } else if (hue_segment >= 1 && hue_segment < 2) {
        r1 = x;
        g1 = chroma;
        b1 = 0;
    } else if (hue_segment >= 2 && hue_segment < 3) {
        r1 = 0;
        g1 = chroma;
        b1 = x;
    } else if (hue_segment >= 3 && hue_segment < 4) {
        r1 = 0;
        g1 = x;
        b1 = chroma;
    } else if (hue_segment >= 4 && hue_segment < 5) {
        r1 = x;
        g1 = 0;
        b1 = chroma;
    } else {
        r1 = chroma;
        g1 = 0;
        b1 = x;
    }

    float m = brightness - chroma;
    r = (r1 + m) * 255;
    g = (g1 + m) * 255;
    b = (b1 + m) * 255;
    hue = r;
    saturation = g;
    brightness = b;
}

void hexagon_RGB::changeLight(float value) {
    if (std::abs(c1 - value) > 0.02) {
        a1 = value;
        b1 = value;
        c1 = value;
        setTexture();
    }
}

void hexagon_CMY::changeLight(float value) {
    if (std::abs(c1 - value) > 0.02) {
        a1 = value;
        b1 = value;
        c1 = value;
        setTexture();
    }
}

void hexagon_HSL::changeLight(float value) {
    if (std::abs(c1 - value) > 0.02) {
        c1 = value;
        setTexture();
    }
}

void hexagon_HSB::changeLight(float value) {
    if (std::abs(c1 - value) > 0.02) {
        c1 = value;
        setTexture();
    }
}