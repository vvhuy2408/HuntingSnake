#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <stack>
#include <iostream>
#include <vector>

#include "Design/textureManager.hpp"
#include "Design/effects.hpp"
#include "Design/interface.hpp"
#include "Design/screenState.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hunting Snake");

    // Backgrounds
    sf::Sprite start_game = loadInterface("Design/Assets/startgame.png", "start-game", window);
    sf::Sprite main_menu = loadInterface("Design/Assets/mainmenu.png", "main-menu", window);
    sf::Sprite in_game = loadInterface("Design/Assets/ingame.png", "in-game", window);
    sf::Sprite about_us = loadInterface("Design/Assets/info.png", "about-us", window);
    sf::Sprite load_game = loadInterface("Design/Assets/loadgame.png", "load-game", window);

    // Buttons
    Button start_button = createButton("Design/Assets/button/start.png", "", 640, 550);
    Button soundOn_button = createButton("Design/Assets/button/sound_on.png", "", 48, 48);
    Button soundOff_button = createButton("Design/Assets/button/sound_off.png", "", 48, 48);
    Button back_button = createButton("Design/Assets/button/arrow_left.png", "", 148, 48);
    Button heart_button = createButton("Design/Assets/button/heart_button.png", "", 500, 428);
    Button loadgame_button = createButton("Design/Assets/button/loadgame.png", "Design/Assets/button/loadgame_press.png", 980, 169);
    Button howto_button = createButton("Design/Assets/button/howto.png", "Design/Assets/button/howto_press.png", 980, 369);
    Button about_button = createButton("Design/Assets/button/about.png", "Design/Assets/button/about_press.png", 980, 569);

    std::vector<Button> slots;
    float x_slot = 174, y_slot = 247;
    for (int i = 0; i < 6; i++)
    {
        if (i % 2 != 0)
        {
            x_slot += 333;
            Button slot = createButton("Design/Assets/button/slot.png", "", x_slot, y_slot);
            slots.push_back(slot);
            x_slot -= 333;
        } 
        
        else if (i % 2 == 0)
        {   
            if (i != 0)
                y_slot += 53;
                
            Button slot = createButton("Design/Assets/button/slot.png", "", x_slot, y_slot);
            slots.push_back(slot);
        }
    }

    std::vector<Button> rocks;
    for (int i = 0; i < 3; i++)
    {
        Button rock = createButton("Design/Assets/button/rock.png", "", 48, 48);
        rocks.push_back(rock);
    }

    loadgame_button.hoverOffset = sf::Vector2f(35.f, 0.f);
    howto_button.hoverOffset = sf::Vector2f(35.f, 0.f);
    about_button.hoverOffset = sf::Vector2f(35.f, 0.f);

    // Music
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Design/Music/background.ogg"))
        std::cout << "Error loading background music!\n";
    else
    {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(60);
        backgroundMusic.play();
    }

    bool isExit = false;
    bool soundOff = false;

    // Initialize the screen stack once
    std::stack<ScreenState> screenStack;
    screenStack.push(ScreenState::StartMenu);

    // Font
    sf::Font font;
    if (!font.loadFromFile("Design/Assets/Font/ThaleahFat.ttf"))
    {
        std::cout << "Error loading font!\n";
    }

    // Clock
    sf::Clock clock;
    sf::Time duration01 = sf::seconds(30); // input thời gian ở đây nhé

    // =================== MAIN LOOP ===================
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

        window.clear();
        ScreenState currentScreen = screenStack.top();

        // =================== SWITCH BETWEEN SCREENS ===================
        switch (currentScreen)
        {
        case ScreenState::StartMenu:
            window.draw(start_game);
            updateButton(start_button, window);
            drawButton(window, start_button);

            if (start_button.isClicked)
                changeScreen(screenStack, ScreenState::MainMenu);
            break;

        case ScreenState::MainMenu:
            window.draw(main_menu);

            updateButton(back_button, window);
            drawButton(window, back_button);

            updateButton(heart_button, window);
            drawButton(window, heart_button);

            updateButton(loadgame_button, window);
            drawButton(window, loadgame_button);

            updateButton(howto_button, window);
            drawButton(window, howto_button);

            updateButton(about_button, window);
            drawButton(window, about_button);

            if (back_button.isClicked)
                goBack(screenStack);

            if (howto_button.isClicked)
                changeScreen(screenStack, ScreenState::HowTo);

            if (about_button.isClicked)
                changeScreen(screenStack, ScreenState::About);

            if (heart_button.isClicked)
                changeScreen(screenStack, ScreenState::InGame);

            if (loadgame_button.isClicked)
                changeScreen(screenStack, ScreenState::LoadGame);
            break;

        case ScreenState::InGame:
            window.draw(in_game);
            updateButton(back_button, window);
            drawButton(window, back_button);

            if (back_button.isClicked)
                goBack(screenStack);

            break;

        case ScreenState::About:
            window.draw(about_us);
            updateButton(back_button, window);
            drawButton(window, back_button);

            if (back_button.isClicked)
                goBack(screenStack);
            break;
        case ScreenState::LoadGame:
            window.draw(load_game);

            drawButton(window, back_button);

            if (back_button.isClicked)
                goBack(screenStack);

            for (int i = 0; i < slots.size(); i++)
            {
                updateButton(slots[i], window);
                drawButton(window, slots[i]);
            }

            updateButton(back_button, window);
            drawButton(window, back_button);

            break;

        default:
            break;
        }

        // =================== SOUND TOGGLE (GLOBAL) ===================
        if (soundOff)
        {
            updateButton(soundOff_button, window);
            drawButton(window, soundOff_button);
            if (soundOff_button.isClicked)
            {
                soundOff = false;
                backgroundMusic.play();
            }
        }
        else
        {
            updateButton(soundOn_button, window);
            drawButton(window, soundOn_button);
            if (soundOn_button.isClicked)
            {
                soundOff = true;
                backgroundMusic.pause();
            }
        }

        window.display();
    }

    return 0;
}
