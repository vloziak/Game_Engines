#include "Food.h"
#include <random>

Food::Food() {}

bool Food::load(const std::string& path) {
    if (!texture.loadFromFile(path)) return false;
    sprite = std::make_unique<sf::Sprite>(texture);
    image = texture.copyToImage();
    auto ts = texture.getSize();
    if (ts.x > 0 && ts.y > 0) {
        float maxDim = static_cast<float>(std::max(ts.x, ts.y));
        float scale = 48.f / maxDim;
        sprite->setScale(sf::Vector2f(scale, scale));
        sprite->setOrigin(sf::Vector2f(static_cast<float>(ts.x) / 2.f, static_cast<float>(ts.y) / 2.f));
    }
    return true;
}

void Food::respawn(const sf::Vector2u& windowSize, std::uint32_t seed) {
    auto bounds = getGlobalBounds();
    float halfW = bounds.size.x / 2.f;
    float halfH = bounds.size.y / 2.f;
    float minX = halfW;
    float minY = halfH;
    float maxX = static_cast<float>(windowSize.x) - halfW;
    float maxY = static_cast<float>(windowSize.y) - halfH;
    if (maxX < minX) maxX = minX;
    if (maxY < minY) maxY = minY;

    std::mt19937 rng(seed ? seed : static_cast<std::uint32_t>(std::random_device{}()));
    std::uniform_real_distribution<float> dx(minX, maxX);
    std::uniform_real_distribution<float> dy(minY, maxY);
    setPosition(sf::Vector2f(dx(rng), dy(rng)));
}

void Food::render(sf::RenderTarget& target) const {
    if (sprite) target.draw(*sprite);
}

sf::Vector2f Food::getPosition() const {
    return sprite ? sprite->getPosition() : sf::Vector2f();
}

void Food::setPosition(const sf::Vector2f& pos) {
    if (sprite) sprite->setPosition(pos);
}

sf::FloatRect Food::getGlobalBounds() const {
    return sprite ? sprite->getGlobalBounds() : sf::FloatRect();
}


