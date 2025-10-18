#include <SFML/Graphics.hpp>
#include <iostream>

class Game {
public:
    Game()
    : window(sf::VideoMode({800, 600}), "Assignment 2 - Game Loop"),
      player(25.f),
      font(),
      startText(font, "", 24),
      exitText(font, "", 20),
      pauseText(font, "", 28)
    {
        window.setFramerateLimit(60);

        if (!font.openFromFile("../assets/fonts/PressStart2P-Regular.ttf")) {
            std::cerr << "Error: Could not load font!" << std::endl;
            exit(-1);
        }

        startText.setFont(font);
        startText.setString("Press ENTER to start");
        startText.setCharacterSize(24);
        startText.setFillColor(sf::Color::Black);
        startText.setPosition({150.f, 300.f});

        exitText.setFont(font);
        exitText.setString("Press Q to exit");
        exitText.setCharacterSize(20);
        exitText.setFillColor(sf::Color::Red);
        exitText.setPosition({10.f, 10.f});

        pauseText.setFont(font);
        pauseText.setString("PAUSED");
        pauseText.setCharacterSize(28);
        pauseText.setFillColor(sf::Color::Blue);
        pauseText.setPosition({320.f, 260.f});

        player.setFillColor(sf::Color::Blue);
        player.setPosition({400.f - player.getRadius(), 300.f - player.getRadius()});
    }

    void run() {
        while (window.isOpen()) {
            float deltaTime = deltaClock.restart().asSeconds();
            handleEvents();
            if (gameStarted && !paused && window.hasFocus()) {
                update(deltaTime);
            }
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text startText, exitText, pauseText;
    sf::CircleShape player;
    sf::Clock deltaClock;

    bool gameStarted = false;
    bool paused = false;
    const float speed = 200.f;

    void handleEvents() {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->scancode) {
                    case sf::Keyboard::Scancode::Q:
                    case sf::Keyboard::Scancode::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::Scancode::Enter:
                        if (!gameStarted) gameStarted = true;
                        break;
                    case sf::Keyboard::Scancode::P:
                        if (gameStarted) paused = !paused;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void update(float deltaTime) {
        sf::Vector2f moveDir(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) moveDir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) moveDir.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) moveDir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) moveDir.x += 1.f;

        if (moveDir.x != 0.f || moveDir.y != 0.f) {
            float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
            moveDir /= len;
        }

        sf::Vector2f newPos = player.getPosition() + moveDir * speed * deltaTime;

        float radius = player.getRadius();
        if (newPos.x < 0) newPos.x = 0;
        if (newPos.y < 0) newPos.y = 0;
        if (newPos.x + radius * 2 > window.getSize().x)
            newPos.x = window.getSize().x - radius * 2;
        if (newPos.y + radius * 2 > window.getSize().y)
            newPos.y = window.getSize().y - radius * 2;

        player.setPosition(newPos);
    }

    void render() {
        window.clear(sf::Color::White);

        if (!gameStarted) {
            window.draw(startText);
        } else {
            window.draw(player);
            window.draw(exitText);
            if (paused)
                window.draw(pauseText);
        }

        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
