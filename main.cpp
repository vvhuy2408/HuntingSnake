#include <SFML/Graphics.hpp>
#include <iostream>

#include "Design/textureManager.hpp"
#include "Design/effects.hpp"
#include "Design/interface.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hunting Snake");

    // Background
    sf::Sprite start_game = loadInterface("Design/Assets/startgame.png", "start-game", window);
    sf::Sprite main_menu = loadInterface("Design/Assets/mainmenu.png", "main-menu", window);

    // Buttons
    Button start_button = creatButton("Design/Assets/button/start.png", "start-game-button", 640, 550);
    Button soundOn_button = creatButton("Design/Assets/button/sound_on.png", "soundOn-game-button", 48, 48);
    Button soundOff_button = creatButton("Design/Assets/button/sound_off.png", "soundOff-game-button", 48, 48);

    bool isExit = false;
    bool inGame = false;
    bool soundOff = false;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                isExit = true;

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                isExit = true;
        }

        if (isExit)
        {
            window.clear();
            exitEffect(window, isExit);
            window.display();
            continue;
        }

        if (inGame)
        {
            window.clear();
            window.draw(main_menu);
            if (!soundOff)
            {
                drawButton(window, soundOn_button);
                updateButton(soundOn_button, window);
            }
            else
            {
                drawButton(window, soundOff_button);
            }
            window.display();

            if (soundOff)
            {
                drawButton(window, soundOff_button);
                updateButton(soundOff_button, window);
                if (soundOff_button.isClicked)
                    soundOff = false;
            }
            else
            {
                drawButton(window, soundOn_button);
                updateButton(soundOn_button, window);
                if (soundOn_button.isClicked)
                    soundOff = true;
            }
            continue;
        }

        updateButton(start_button, window);

        if (soundOff)
        {
            drawButton(window, soundOff_button);
            updateButton(soundOff_button, window);
            if (soundOff_button.isClicked)
                soundOff = false;
        }
        else
        {
            drawButton(window, soundOn_button);
            updateButton(soundOn_button, window);
            if (soundOn_button.isClicked)
                soundOff = true;
        }

        if (start_button.isClicked)
        {
            inGame = true;
        }

        window.clear();
        window.draw(start_game);
        drawButton(window, start_button);

        if (soundOff)
            drawButton(window, soundOff_button);
        else
            drawButton(window, soundOn_button);

        window.display();
    }

    return 0;
}