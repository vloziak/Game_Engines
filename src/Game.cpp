#include "Game.h"
#include "Player.h"
#include "Button.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdint>

Game::Game()
    : window(sf::VideoMode({800, 600}), "Game Loop")
    , startText(font, "", 24)
    , exitText(font, "", 20)
    , pauseText(font, "", 28)
    , scoreText(font, "", 20) {
    window.setFramerateLimit(60);

    if (!font.openFromFile("../assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        std::exit(-1);
    }

    startText.setFont(font);
    startText.setString("Press ENTER to start");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::Black);
    startText.setPosition({150.f, 300.f});

    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(28);
    pauseText.setFillColor(sf::Color::Blue);
    pauseText.setPosition({320.f, 260.f});

    player = new Player();

    if (menuBackgroundTexture.loadFromFile("../assets/img/menu_bg.jpeg")) {
        menuBackgroundSprite = std::make_unique<sf::Sprite>(menuBackgroundTexture);
        layoutMenuBackground();
    }

    sf::Vector2f center(static_cast<float>(window.getSize().x) / 2.f,
                        static_cast<float>(window.getSize().y) / 2.f);
    auto startBtn = std::make_unique<Button>(font, "Start Game", sf::Vector2f(240.f, 60.f), center + sf::Vector2f(0.f, -30.f));
    startBtn->setCallback([this]() {
        state = State::Playing;
        gameStarted = true;
    });
    startBtn->setColors(sf::Color(230, 230, 230), sf::Color(250, 250, 250), sf::Color::Black);

    auto quitBtn = std::make_unique<Button>(font, "Quit", sf::Vector2f(240.f, 60.f), center + sf::Vector2f(0.f, 50.f));
    quitBtn->setCallback([this]() { window.close(); });
    quitBtn->setColors(sf::Color(230, 230, 230), sf::Color(250, 250, 250), sf::Color::Black);

    menuButtons.emplace_back(std::move(startBtn));
    menuButtons.emplace_back(std::move(quitBtn));

    exitButton = std::make_unique<Button>(font, "exit", sf::Vector2f(200.f, 50.f), sf::Vector2f(120.f, 35.f));
    exitButton->setColors(sf::Color(230, 230, 230), sf::Color(250, 250, 250), sf::Color::Black);
    exitButton->setCallback([this]() {
        state = State::Menu;
        paused = false;
        gameStarted = false;
    });

    if (gameBackgroundTexture.loadFromFile("../assets/img/game_bg.jpeg")) {
        gameBackgroundSprite = std::make_unique<sf::Sprite>(gameBackgroundTexture);
        layoutGameBackground();
    }

    (void)food.load("../assets/img/food.png");
    food.respawn(window.getSize());
    score = 0;
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Black);
    layoutScoreText();
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        handleEvents();
        if (gameStarted && !paused && window.hasFocus()) {
            update(deltaTime);
        }
        render();
    }
}

void Game::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (state == State::Menu) {
            if (auto* moved = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2f mp(static_cast<float>(moved->position.x), static_cast<float>(moved->position.y));
                for (auto& b : menuButtons) b->handleMouseMove(mp);
            }
            if (auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (pressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mp(static_cast<float>(pressed->position.x), static_cast<float>(pressed->position.y));
                    for (auto& b : menuButtons) b->handleMousePress(mp);
                }
            }
        }

        if (auto* resized = event->getIf<sf::Event::Resized>()) {
            (void)resized;
            layoutMenuBackground();
            layoutGameBackground();
            layoutScoreText();
        }

        if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->scancode) {
                case sf::Keyboard::Scancode::Enter:
                    if (state == State::Menu) { state = State::Playing; gameStarted = true; }
                    break;
                case sf::Keyboard::Scancode::P:
                    if (state == State::Playing && gameStarted) paused = !paused;
                    break;
                default:
                    break;
            }
            if (state == State::Playing) player->handleKeyPressed(keyPressed->scancode);
        }

        if (auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (state == State::Playing) player->handleKeyReleased(keyReleased->scancode);
        }

        if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                std::cout << "Left mouse button pressed at (" << mousePressed->position.x
                          << ", " << mousePressed->position.y << ")\n";
                if (state == State::Playing && exitButton) {
                    sf::Vector2f mp(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
                    exitButton->handleMousePress(mp);
                }
            } else if (mousePressed->button == sf::Mouse::Button::Right) {
                std::cout << "Right mouse button pressed\n";
            }
        }

        if (auto* moved = event->getIf<sf::Event::MouseMoved>()) {
            if (state == State::Playing && exitButton) {
                sf::Vector2f mp(static_cast<float>(moved->position.x), static_cast<float>(moved->position.y));
                exitButton->handleMouseMove(mp);
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (state == State::Playing)
        player->update(deltaTime, window.getSize());

    if (state == State::Playing) {
        auto foodSprite = food.getSprite();
        auto foodImg = food.getCollisionImage();
        if (foodSprite && foodImg) {
            bool collided = false;
            if (player->hasTextureSprite()) {
                const sf::Sprite* pSpr = player->getSprite();
                const sf::Image* pImg = player->getCollisionImage();
                auto aBounds = pSpr->getGlobalBounds();
                auto bBounds = foodSprite->getGlobalBounds();
                float left = std::max(aBounds.position.x, bBounds.position.x);
                float top = std::max(aBounds.position.y, bBounds.position.y);
                float right = std::min(aBounds.position.x + aBounds.size.x, bBounds.position.x + bBounds.size.x);
                float bottom = std::min(aBounds.position.y + aBounds.size.y, bBounds.position.y + bBounds.size.y);
                if (right > left && bottom > top) {
                    sf::FloatRect inter({left, top}, {right - left, bottom - top});
                    for (int x = static_cast<int>(inter.position.x); x < static_cast<int>(inter.position.x + inter.size.x) && !collided; ++x) {
                        for (int y = static_cast<int>(inter.position.y); y < static_cast<int>(inter.position.y + inter.size.y) && !collided; ++y) {
                            sf::Vector2f wp(static_cast<float>(x), static_cast<float>(y));
                            sf::Vector2f aLocal = pSpr->getInverseTransform().transformPoint(wp);
                            sf::Vector2f bLocal = foodSprite->getInverseTransform().transformPoint(wp);
                            auto aTexSize = pImg->getSize();
                            auto bTexSize = foodImg->getSize();
                            if (aLocal.x >= 0 && aLocal.y >= 0 && aLocal.x < static_cast<float>(aTexSize.x) && aLocal.y < static_cast<float>(aTexSize.y) &&
                                bLocal.x >= 0 && bLocal.y >= 0 && bLocal.x < static_cast<float>(bTexSize.x) && bLocal.y < static_cast<float>(bTexSize.y)) {
                                auto aPx = pImg->getPixel(sf::Vector2u(static_cast<unsigned>(aLocal.x), static_cast<unsigned>(aLocal.y))).a;
                                auto bPx = foodImg->getPixel(sf::Vector2u(static_cast<unsigned>(bLocal.x), static_cast<unsigned>(bLocal.y))).a;
                                if (aPx > 10 && bPx > 10) {
                                    collided = true;
                                }
                            }
                        }
                    }
                }
            } else {
                auto foodBounds = food.getGlobalBounds();
                auto playerPos = player->getPosition();
                float approxRadius = player->getRadius();
                float cx = std::max(foodBounds.position.x, std::min(playerPos.x, foodBounds.position.x + foodBounds.size.x));
                float cy = std::max(foodBounds.position.y, std::min(playerPos.y, foodBounds.position.y + foodBounds.size.y));
                float dx = playerPos.x - cx;
                float dy = playerPos.y - cy;
                collided = (dx * dx + dy * dy <= approxRadius * approxRadius);
            }

            if (collided) {
                score += 1;
                scoreText.setString("Score: " + std::to_string(score));
                layoutScoreText();
                food.respawn(window.getSize());
            }
        }
    }
}

void Game::render() {
    window.clear(sf::Color::White);

    if (state == State::Menu) {
        if (menuBackgroundSprite) window.draw(*menuBackgroundSprite);
        for (const auto& b : menuButtons) b->render(window);
    } else {
        if (gameBackgroundSprite) window.draw(*gameBackgroundSprite);
        food.render(window);
        player->render(window);
        if (exitButton) exitButton->render(window);
        if (paused) window.draw(pauseText);
        window.draw(scoreText);
    }

    window.display();
}

void Game::layoutMenuBackground() {
    if (!menuBackgroundSprite) return;
    auto ws = window.getSize();
    auto ts = menuBackgroundTexture.getSize();
    if (ts.x == 0 || ts.y == 0) return;

    float scaleX = static_cast<float>(ws.x) / static_cast<float>(ts.x);
    float scaleY = static_cast<float>(ws.y) / static_cast<float>(ts.y);
    float scale = std::max(scaleX, scaleY);
    menuBackgroundSprite->setScale(sf::Vector2f(scale, scale));

    auto gb = menuBackgroundSprite->getGlobalBounds();
    menuBackgroundSprite->setPosition(sf::Vector2f(
        (static_cast<float>(ws.x) - gb.size.x) / 2.f,
        (static_cast<float>(ws.y) - gb.size.y) / 2.f
    ));
}

void Game::layoutGameBackground() {
    if (!gameBackgroundSprite) return;
    auto ws = window.getSize();
    auto ts = gameBackgroundTexture.getSize();
    if (ts.x == 0 || ts.y == 0) return;

    float scaleX = static_cast<float>(ws.x) / static_cast<float>(ts.x);
    float scaleY = static_cast<float>(ws.y) / static_cast<float>(ts.y);
    float scale = std::max(scaleX, scaleY);
    gameBackgroundSprite->setScale(sf::Vector2f(scale, scale));

    auto gb = gameBackgroundSprite->getGlobalBounds();
    gameBackgroundSprite->setPosition(sf::Vector2f(
        (static_cast<float>(ws.x) - gb.size.x) / 2.f,
        (static_cast<float>(ws.y) - gb.size.y) / 2.f
    ));
}

void Game::layoutScoreText() {
    auto ws = window.getSize();
    auto b = scoreText.getLocalBounds();
    float x = static_cast<float>(ws.x) - 10.f - b.size.x;
    float y = static_cast<float>(ws.y) - 10.f - b.size.y;
    scoreText.setPosition({x, y});
}


