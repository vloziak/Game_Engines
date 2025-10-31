#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class RoundedRectangleShape : public sf::Shape {
public:
    RoundedRectangleShape(const sf::Vector2f& size = {0.f, 0.f}, float radius = 0.f, std::size_t cornerPointCount = 10);

    void setSize(const sf::Vector2f& size);
    const sf::Vector2f& getSize() const;

    void setRadius(float radius);
    float getRadius() const;

    void setCornerPointCount(std::size_t count);
    std::size_t getCornerPointCount() const;

    virtual std::size_t getPointCount() const override;
    virtual sf::Vector2f getPoint(std::size_t index) const override;

private:
    sf::Vector2f m_size;
    float m_radius;
    std::size_t m_cornerPointCount;
};

class Button {
public:
    Button(const sf::Font& font,
           const std::string& label,
           const sf::Vector2f& size,
           const sf::Vector2f& centerPosition);

    void setCallback(std::function<void()> onClick);
    void setColors(const sf::Color& fill, const sf::Color& hoverFill, const sf::Color& textColor);

    void handleMouseMove(const sf::Vector2f& mousePos);
    void handleMousePress(const sf::Vector2f& mousePos);

    void render(sf::RenderTarget& target) const;

    bool contains(const sf::Vector2f& point) const;

private:
    RoundedRectangleShape rect;
    sf::Text text;
    sf::Color baseFill {200, 200, 200};
    sf::Color hoverFill {220, 220, 220};
    sf::Color textColor {20, 20, 20};
    bool hovered = false;
    std::function<void()> onClick;
};


