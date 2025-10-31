#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Food {
public:
    Food();

    bool load(const std::string& path);

    void respawn(const sf::Vector2u& windowSize, std::uint32_t seed = 0);

    void render(sf::RenderTarget& target) const;

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);
    sf::FloatRect getGlobalBounds() const;
    const sf::Sprite* getSprite() const { return sprite.get(); }
    const sf::Image* getCollisionImage() const { return sprite ? &image : nullptr; }

private:
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    sf::Image image;
};


