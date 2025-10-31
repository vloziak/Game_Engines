#include "Button.h"

RoundedRectangleShape::RoundedRectangleShape(const sf::Vector2f& size, float radius, std::size_t cornerPointCount)
    : m_size(size), m_radius(radius), m_cornerPointCount(cornerPointCount) {
    update();
}

void RoundedRectangleShape::setSize(const sf::Vector2f& size) {
    m_size = size;
    update();
}

const sf::Vector2f& RoundedRectangleShape::getSize() const {
    return m_size;
}

void RoundedRectangleShape::setRadius(float radius) {
    m_radius = radius;
    update();
}

float RoundedRectangleShape::getRadius() const {
    return m_radius;
}

void RoundedRectangleShape::setCornerPointCount(std::size_t count) {
    m_cornerPointCount = count;
    update();
}

std::size_t RoundedRectangleShape::getCornerPointCount() const {
    return m_cornerPointCount;
}

std::size_t RoundedRectangleShape::getPointCount() const {
    return m_cornerPointCount * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const {
    float r = std::min({m_radius, m_size.x / 2.f, m_size.y / 2.f});
    if (r <= 0.f) {
        switch (index) {
            case 0: return {0.f, 0.f};
            case 1: return {m_size.x, 0.f};
            case 2: return {m_size.x, m_size.y};
            default: return {0.f, m_size.y};
        }
    }

    std::size_t corner = index / m_cornerPointCount;
    std::size_t i = index % m_cornerPointCount;
    float angle = static_cast<float>(i) * 90.f / static_cast<float>(m_cornerPointCount - 1);
    float rad = angle * 3.141592654f / 180.f;

    sf::Vector2f center;
    switch (corner) {
        case 0: center = {m_size.x - r, r}; angle = 270.f + angle; break;
        case 1: center = {m_size.x - r, m_size.y - r}; angle = 0.f + angle; break;
        case 2: center = {r, m_size.y - r}; angle = 90.f + angle; break;
        default: center = {r, r}; angle = 180.f + angle; break;
    }

    rad = angle * 3.141592654f / 180.f;
    return {center.x + std::cos(rad) * r, center.y + std::sin(rad) * r};
}

Button::Button(const sf::Font& font,
               const std::string& label,
               const sf::Vector2f& size,
               const sf::Vector2f& centerPosition)
    : text(font, "", 18) {
    rect.setSize(size);
    rect.setRadius(std::min(size.x, size.y) * 0.25f);
    rect.setCornerPointCount(14);
    rect.setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
    rect.setPosition(centerPosition);
    rect.setFillColor(baseFill);
    rect.setOutlineThickness(3.f);
    rect.setOutlineColor(sf::Color::Black);

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(textColor);

    auto bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f,
                                bounds.position.y + bounds.size.y / 2.f));
    text.setPosition(centerPosition);
}

void Button::setCallback(std::function<void()> cb) {
    onClick = std::move(cb);
}

void Button::setColors(const sf::Color& fill, const sf::Color& hover, const sf::Color& txt) {
    baseFill = fill;
    hoverFill = hover;
    textColor = txt;
    rect.setFillColor(hovered ? hoverFill : baseFill);
    text.setFillColor(textColor);
}

void Button::handleMouseMove(const sf::Vector2f& mousePos) {
    bool nowHover = contains(mousePos);
    if (nowHover != hovered) {
        hovered = nowHover;
        rect.setFillColor(hovered ? hoverFill : baseFill);
    }
}

void Button::handleMousePress(const sf::Vector2f& mousePos) {
    if (contains(mousePos) && onClick) {
        onClick();
    }
}

bool Button::contains(const sf::Vector2f& point) const {
    auto r = rect.getGlobalBounds();
    return point.x >= r.position.x && point.x <= r.position.x + r.size.x &&
           point.y >= r.position.y && point.y <= r.position.y + r.size.y;
}

void Button::render(sf::RenderTarget& target) const {
    target.draw(rect);
    target.draw(text);
}


