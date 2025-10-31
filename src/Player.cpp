#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player()
    : circle(25.f) {
    if (texture.loadFromFile("../assets/img/player.png")) {
        hasTexture = true;
        sprite = std::make_unique<sf::Sprite>(texture);
        collisionImage = texture.copyToImage();
        auto texSize = texture.getSize();
        sprite->setOrigin(sf::Vector2f(static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y) / 2.f));
        sprite->setPosition({400.f, 300.f});
        const float maxDisplaySize = 96.f;
        float maxTexDim = static_cast<float>(std::max(texSize.x, texSize.y));
        if (maxTexDim > 0.f) {
            float uniformScale = maxDisplaySize / maxTexDim;
            baseScale = uniformScale;
            sprite->setScale(sf::Vector2f(baseScale, baseScale));
        }
    } else {
        hasTexture = false;
        circle.setFillColor(sf::Color::Blue);
        circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
        circle.setPosition({400.f, 300.f});
    }
}

void Player::handleKeyPressed(sf::Keyboard::Scancode code) {
    switch (code) {
        case sf::Keyboard::Scancode::W: moveUp = true; break;
        case sf::Keyboard::Scancode::S: moveDown = true; break;
        case sf::Keyboard::Scancode::A: moveLeft = true; break;
        case sf::Keyboard::Scancode::D: moveRight = true; break;
        default: break;
    }
}

void Player::handleKeyReleased(sf::Keyboard::Scancode code) {
    switch (code) {
        case sf::Keyboard::Scancode::W: moveUp = false; break;
        case sf::Keyboard::Scancode::S: moveDown = false; break;
        case sf::Keyboard::Scancode::A: moveLeft = false; break;
        case sf::Keyboard::Scancode::D: moveRight = false; break;
        default: break;
    }
}

void Player::update(float deltaTime, const sf::Vector2u& windowSize) {
    sf::Vector2f moveDir(0.f, 0.f);
    if (moveUp) moveDir.y -= 1.f;
    if (moveDown) moveDir.y += 1.f;
    if (moveLeft) moveDir.x -= 1.f;
    if (moveRight) moveDir.x += 1.f;

    if (moveDir.x != 0.f || moveDir.y != 0.f) {
        float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir /= len;
    }

    if (hasTexture && moveDir.x != 0.f) {
        float sx = (moveDir.x < 0.f) ? -baseScale : baseScale;
        sprite->setScale(sf::Vector2f(sx, baseScale));
    }

    sf::Vector2f pos = getPosition();
    pos += moveDir * speed * deltaTime;

    float halfWidth;
    float halfHeight;
    if (hasTexture) {
        auto bounds = sprite->getGlobalBounds();
        halfWidth = bounds.size.x / 2.f;
        halfHeight = bounds.size.y / 2.f;
    } else {
        halfWidth = circle.getRadius();
        halfHeight = circle.getRadius();
    }

    if (pos.x - halfWidth < 0) pos.x = halfWidth;
    if (pos.y - halfHeight < 0) pos.y = halfHeight;
    if (pos.x + halfWidth > static_cast<float>(windowSize.x)) pos.x = static_cast<float>(windowSize.x) - halfWidth;
    if (pos.y + halfHeight > static_cast<float>(windowSize.y)) pos.y = static_cast<float>(windowSize.y) - halfHeight;

    setPosition(pos);
}

void Player::render(sf::RenderTarget& target) const {
    if (hasTexture) {
        target.draw(*sprite);
    } else {
        target.draw(circle);
    }
}

void Player::setPosition(const sf::Vector2f& position) {
    if (hasTexture) {
        sprite->setPosition(position);
    } else {
        circle.setPosition(position);
    }
}

sf::Vector2f Player::getPosition() const {
    if (hasTexture) {
        return sprite->getPosition();
    }
    return circle.getPosition();
}

float Player::getRadius() const {
    return circle.getRadius();
}


