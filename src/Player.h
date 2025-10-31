#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Player {
public:
    Player();

    void handleKeyPressed(sf::Keyboard::Scancode code);
    void handleKeyReleased(sf::Keyboard::Scancode code);

    void update(float deltaTime, const sf::Vector2u& windowSize);
    void render(sf::RenderTarget& target) const;

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    float getRadius() const;

    bool hasTextureSprite() const { return hasTexture; }
    const sf::Sprite* getSprite() const { return sprite.get(); }
    const sf::Image* getCollisionImage() const { return hasTexture ? &collisionImage : nullptr; }

private:
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    bool hasTexture = false;
    sf::Image collisionImage;

    sf::CircleShape circle;

    bool moveUp = false;
    bool moveDown = false;
    bool moveLeft = false;
    bool moveRight = false;

    float speed = 200.f;
    float baseScale = 1.f;
};


