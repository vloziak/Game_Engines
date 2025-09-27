#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <iostream>

int main() {
    // GLM
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 5.0f, 6.0f);

    glm::vec3 sum = a + b;

    std::cout << "vector a:(" << a.x << "," << a.y << "," << a.z << ")"<< std::endl;
    std::cout << "vector b:(" << b.x << "," << b.y << "," << b.z << ")"<< std::endl;
    std::cout << "sum:(" << sum.x << "," << sum.y << "," << sum.z << ")"<< std::endl;

    //SFML
    sf::RenderWindow window(sf::VideoMode({800, 600}),"HW 1");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(sf::Vector2f((window.getSize().x-shape.getRadius()*2)/2,(window.getSize().y-shape.getRadius()*2)/2));

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();}

        window.clear(sf::Color::White);

        window.draw(shape);
        window.display();
    }
    return 0;
}
