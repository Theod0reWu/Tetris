#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "tetrominoes.h"
#include <SFML/Audio.hpp>

class game {
public:
	//matrix size
	static const int height = 20;
	static const int width = 10;
	static const int borderThickness = 5;

	game(int x, int y, bool musicOn = true);

	bool lostYet() { return lost; }

	void loadTextures();

	void draw( sf::RenderWindow& window);

	void update(sf::Clock& clock);
	void randomAdd();
	void randNextAdd();
	void add(int num);

	void moveRight();
	void moveLeft();
	void rotateRight();
	void rotateLeft();

	void softDropOn() {softDrop = true;}
	void softDropOff() {softDrop = false;}

	void hardDropOn() { hardDrop = true; }
	void hardDropOff() { hardDrop = false; }
	void readyHardDrop() { hardDropReady = true; }

	sf::Vector2f convertToBoard();

	void addToBoard();
	void restoreRotate();

	void hold();

	void pause();
	void resume();

	void togglePause();
	
	/*void restart() {
		game(globalX - block::size * 6, globalY);
	}*/

	//ai stuff
	const std::vector<std::vector <block*> >& getBoardState() { return board; }
	int getScore() { return score; }
	int getActive() { return tetrominoes.back().getType(); }
	float getActiveRotation() { return tetrominoes.back().getBlocks()[0].getRect().getRotation(); }

	int* getNextTets() { return nextTet; }

	const std::vector<sf::Vector2i>& getActiveBP() { return tetrominoes.back().getBoardPoints(); }
	int getLastX() { return lastX; }
	int getLastY() { return lastY; }
	int getGlobalX() { return globalX; }
	int getGlobalY() { return globalY; }
	int getWidth() { return width; }
	int getHeight() { return height; }

	int getLevel() { return level; }
	int getLinesCleared() { return cleared; }

	~game() {
		delete ghostPiece;
	}
private:
	void drawHold(sf::RenderWindow& window, int i);

	int level;
	sf::Time lastMoved;
	bool softDrop;
	bool hardDrop;
	bool hardDropReady;

	sf::Time hitTime;
	bool justHit;

	std::vector<sf::Texture> textures;
	//block* board [height][width];
	std::vector<std::vector <block*> > board;
	int nextTet[3];

	int bag[7] = { 0,1,2,3,4,5,6 };
	int atNum;

	void displayBlocks();
	void shuffle();

	std::vector<tetrominoe> tetrominoes;
	int holdIndex;
	bool canHold;

	//play area
	sf::RectangleShape leftSide;
	sf::RectangleShape rightSide;
	sf::RectangleShape bottom;

	//hold area
	sf::RectangleShape holdSide;
	sf::RectangleShape holdBottom;

	//next area
	sf::RectangleShape nextSide;
	sf::RectangleShape nextBottom;

	int globalX;
	int globalY;
	int lastX;
	int lastY;

	bool lost;
	bool canRotate;

	//int previousTet;

	bool paused;

	unsigned int cleared;
	unsigned int score;

	sf::Font font;
	sf::Text levelText;
	sf::Text scoreText;
	sf::Text lostText;
	sf::Text pausedText;

	//music
	sf::Music music;
	bool musicOn;

	//ghost piece
	tetrominoe* ghostPiece;
};
