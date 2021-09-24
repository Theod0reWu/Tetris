#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <iostream>
#include "game.h"

#include "ai.h"
#include <queue>

int main()
{
    geneticAlgorithm();

    srand(std::time(NULL));
	sf::RenderWindow window(sf::VideoMode(2500, 1300), "Not Tetris");
    window.setFramerateLimit(1200); // call it once, after creating the window

    game mainGame(100, 100, false);
    game aiGame(1250,100, false);

	sf::Event event;
    sf::Clock clock;

    std::queue<std::string> aiActions;

	while (window.isOpen()) {
        //sf::Time start = clock.getElapsedTime();
        while (window.pollEvent(event))
        {
            // check the type of the event...
            switch (event.type)
            {
                // window closed
            case sf::Event::Closed:
                window.close();
                break;

                // key pressed
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    mainGame.togglePause();
                    break;
                case sf::Keyboard::Down:
                    mainGame.softDropOn();
                    break;
                case sf::Keyboard::Up:
                    mainGame.rotateRight();
                    break;
                case sf::Keyboard::Z:
                    mainGame.rotateLeft();
                    break;
                case sf::Keyboard::Space:
                    mainGame.hardDropOn();
                    break;
                case sf::Keyboard::Left:
                    mainGame.moveLeft();
                    break;
                case sf::Keyboard::Right:
                    mainGame.moveRight();
                    break;
                case sf::Keyboard::C:
                    mainGame.hold();
                    break;
                case sf::Keyboard::R:
                    //mainGame.restart();
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code) {
                case sf::Keyboard::Down:
                    mainGame.softDropOff();
                    break;
                case sf::Keyboard::Up:
                    mainGame.restoreRotate();
                    break;
                case sf::Keyboard::Z:
                    mainGame.restoreRotate();
                    break;
                case sf::Keyboard::Space:
                    mainGame.readyHardDrop();
                    mainGame.hardDropOff();
                    break;
                }
                break;

            case sf::Event::LostFocus:
                mainGame.pause();
                break;
                
            case sf::Event::GainedFocus:
                mainGame.resume();
                break;

                // we don't process other types of events
            default:
                break;
            }

            window.clear(sf::Color::Black);

            mainGame.update(clock);
            mainGame.draw(window);

            //ai game
            readAiInput(aiGame, aiActions);
            aiGame.update(clock);
            aiGame.draw(window);

            window.display();
            
        }
        window.clear(sf::Color::Black);

        mainGame.update(clock);
        mainGame.draw(window);

        //ai game
        readAiInput(aiGame, aiActions);
        aiGame.update(clock);
        aiGame.draw(window);

        window.display();
	}

	return 0;
}