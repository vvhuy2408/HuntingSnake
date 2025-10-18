#include <SFML/Graphics.hpp>
#include "Design/textureManager.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hunting Snake", sf::Style::Fullscreen);
    /* loadTexture("Design/Assets/Tilesets/Water.png", "water");
    sf::Sprite water = makeSprite("water", 100, 200); */
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear(sf::Color::Black);
        //window.draw(water);
        window.display();
    }

    return 0;
}