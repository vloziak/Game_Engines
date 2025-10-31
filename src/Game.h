#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Player;
#include "Button.h"
#include "Food.h"

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void layoutMenuBackground();
    void layoutGameBackground();
    void layoutScoreText();

private:
    enum class State { Menu, Playing };

    sf::RenderWindow window;
    sf::Font font;
    sf::Text startText, exitText, pauseText;
    sf::Clock deltaClock;

    bool gameStarted = false;
    bool paused = false;

    Player* player = nullptr;

    State state = State::Menu;
    sf::Texture menuBackgroundTexture;
    std::unique_ptr<sf::Sprite> menuBackgroundSprite;
    std::vector<std::unique_ptr<Button>> menuButtons;

    std::unique_ptr<Button> exitButton;

    sf::Texture gameBackgroundTexture;
    std::unique_ptr<sf::Sprite> gameBackgroundSprite;

    Food food;
    std::uint32_t score = 0;
    sf::Text scoreText;
};


