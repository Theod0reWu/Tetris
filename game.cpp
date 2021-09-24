#include "game.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <algorithm>

game::game(int x, int y, bool musicOn) {
	shuffle();
	textures = std::vector<sf::Texture>(7, sf::Texture());
	loadTextures();
	//srand((unsigned)time(NULL));
	level = 1; holdIndex = -1;
	hardDrop = false; softDrop = false;
	hardDropReady = true;

	globalX = x + block::size * 6;
	globalY = y;
	lastX = width / 2;

	leftSide = sf::RectangleShape(sf::Vector2f(borderThickness, block::size * height));
	rightSide = sf::RectangleShape(sf::Vector2f(borderThickness, block::size * height));
	bottom = sf::RectangleShape(sf::Vector2f(block::size * width, borderThickness));

	leftSide.setPosition(globalX - borderThickness, globalY);
	bottom.setPosition(globalX, globalY + leftSide.getSize().y);
	rightSide.setPosition(globalX + bottom.getSize().x, globalY);

	holdSide = sf::RectangleShape(sf::Vector2f(borderThickness, block::size * 5));
	holdBottom = sf::RectangleShape(sf::Vector2f(block::size * 5, borderThickness));

	nextSide = sf::RectangleShape(sf::Vector2f(borderThickness, block::size * 3 * 3));
	nextBottom = sf::RectangleShape(sf::Vector2f(block::size * 5, borderThickness));

	//previousTet = rand() % 7;
	randNextAdd(); randNextAdd(); randNextAdd();
	randomAdd();
	board = std::vector<std::vector<block*>>(height, std::vector<block*>(width, NULL));
	lost = false; canRotate = true; canHold = true; paused = false; justHit = false;

	cleared = 1;

	if (!font.loadFromFile("arial_narrow_7.ttf")) { printf("could not load font\n"); }
	levelText.setFont(font);
	scoreText.setFont(font);
	pausedText.setFont(font);
	lostText.setFont(font);

	levelText.setPosition(globalX - block::size * 6, globalY + block::size * 6);
	scoreText.setPosition(globalX - block::size * 6, globalY + block::size * 8);
	pausedText.setPosition(globalX + block::size * (width / 2 - 3), globalY + block::size * (height / 2 -1));
	lostText.setPosition(globalX + block::size * (width / 2 - 3), globalY + block::size * (height / 2 - 1));

	levelText.setCharacterSize(block::size);
	scoreText.setCharacterSize(block::size);
	pausedText.setCharacterSize(block::size*2);
	lostText.setCharacterSize(block::size*2);

	pausedText.setString("PAUSED");
	lostText.setString("YOU\nLost");

	lostText.setFillColor(sf::Color::Red);

	atNum = 0;

	this->musicOn = musicOn;
	if (!music.openFromFile("Tetris_theme.ogg")) { printf("could not load music\n"); }
	music.setLoop(true);
	music.setVolume(25);
	if (musicOn) {
		music.play();
	}
}

void game::shuffle() {
	for (int i = 0; i < 7; ++i) {
		std::swap(bag[i], bag[rand() % 7]);
	}
}

void game::randNextAdd() {
	if (atNum >= 7) { atNum = 0; shuffle(); }
	//printf("%d\n", atNum);
	nextTet[0] = nextTet[1];
	nextTet[1] = nextTet[2];
	nextTet[2] = bag[atNum++];
	/*while (num == previousTet) {
		num = rand() % 7;
	}
	previousTet = num;*/
}

void game::randomAdd() {
	int num = nextTet[0];
	randNextAdd();
	add(num);
}

void game::add(int num){
	//printf("%d:%d", globalX, globalY);
	//if (lastX == width) { lastX = width - 1; }
	//else if (lastX <= 1) { lastX = 2; };
	lastX = width / 2; //set to center
	int x = globalX + lastX * block::size, y = globalY + block::size*2;
	lastY = y;
	switch (num) {
	case 0:
		x += 1;
		break;
	case 1:
		x += 1;
		y -= block::size;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		y -= block::size / 2;
		x -= block::size / 2;
		break;
	}
	tetrominoes.push_back( tetrominoe(x, y, globalX, globalY,width, height, tetType(num), textures[num]) );
}

void game::loadTextures() {
	textures[0].loadFromFile("lightblue_block.png");
	textures[1].loadFromFile("yellow_block.png");
	textures[2].loadFromFile("purple_block.png");
	textures[3].loadFromFile("green_block.png");
	textures[4].loadFromFile("red_block.png");
	textures[5].loadFromFile("blue_block.png");
	textures[6].loadFromFile("orange_block.png");
}

void game::drawHold(sf::RenderWindow& window, int i) {
	int x = globalX + block::size * (width + 1 + 2.5);
	int y = globalY + block::size * 2.5 * (i + 1);

	if (nextTet[i] == 1) { y -= block::size * .5; }

	tetrominoe temp(x, y, globalX, globalY, width, height, tetType(nextTet[i]), textures[nextTet[i]]);
	//if (nextTet[i] == 0) { temp.rotateRight(); }
	temp.draw(window);
}

void game::draw(sf::RenderWindow& window) {
	//draw outline of play area
	window.draw(leftSide);
	window.draw(rightSide);
	window.draw(bottom);

	for (int h = 0; h < height; ++h) {
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(globalX, globalY + h * block::size), sf::Color(122,122,122)),
			sf::Vertex(sf::Vector2f(globalX + width * block::size,globalY + h * block::size), sf::Color(122,122,122))
		};

		window.draw(line, 2, sf::Lines);
	}

	for (int w = 0; w < width; ++w) {
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(globalX + w * block::size, globalY), sf::Color(122,122,122)),
			sf::Vertex(sf::Vector2f(globalX + w * block::size, globalY + height * block::size), sf::Color(122,122,122))
		};

		window.draw(line, 2, sf::Lines);
	}

	//draw next blocks area
	nextSide.setPosition(globalX + block::size * (width + 1), globalY);
	window.draw(nextSide);
	nextSide.setPosition(globalX + block::size * (width + 6), globalY);
	window.draw(nextSide);

	nextBottom.setPosition(globalX + block::size * (width + 1), globalY);
	window.draw(nextBottom);
	nextBottom.setPosition(globalX + block::size * (width + 1), globalY + block::size * 9 - borderThickness);
	window.draw(nextBottom);

	for (int i = 0; i < 3; ++i) { drawHold(window, i); }

	//draw hold area
	holdSide.setPosition(globalX - block::size * 6, globalY);
	window.draw(holdSide);
	holdSide.setPosition(globalX - block::size, globalY);
	window.draw(holdSide);

	holdBottom.setPosition(globalX - block::size * 6, globalY);
	window.draw(holdBottom);
	holdBottom.setPosition(globalX - block::size * 6, globalY + block::size * 5 - borderThickness);
	window.draw(holdBottom);

	std::vector<int> deleteList;
	//draw tetrominoes
	for (int i = 0; i < tetrominoes.size()-1; ++i) {
		if (!tetrominoes[i].draw(window)) {
			deleteList.push_back(i);
		}
	}

	//removed unused tetrominoes
	int offset = 0;
	for (int index : deleteList) {
		if (index - offset < holdIndex) { holdIndex--; }
		tetrominoes.erase(tetrominoes.begin() + (index - offset));
		offset++;
	}

	//draw the ghost piece

	//move to update and make a copy constructor to spawn in the same place and not the top
	ghostPiece->draw(window);

	//draw the active piece 
	tetrominoes.back().draw(window);

	// draw level and score
	levelText.setString("Level: " + std::to_string(level));
	window.draw(levelText);
	scoreText.setString("Score: " + std::to_string(score));
	window.draw(scoreText);

	//draw the "you lost" / paused
	if (lost) {
		window.draw(lostText);
	} else if (paused) {
		window.draw(pausedText);
	}
}

void game::update(sf::Clock& clock) {
	if (lost && musicOn) { music.pause(); }
	if (lost || paused) { return; }

	float delay = pow((0.8 - (level - 1) * .007), level - 1);
	if (level > 15) { delay = pow((0.8 - (15 - 1) * .007), 15 - 1); }
	bool couldFall = tetrominoes.back().canFall(board);

	if (couldFall) { justHit = false; }
	if (hardDropReady && hardDrop) {
		while (tetrominoes.back().fall(board)) { score += 2; }
		justHit = true;
		hitTime = clock.getElapsedTime() - sf::seconds(1); 
		couldFall = false;
	}
	else if ( !justHit && (
		(softDrop && (clock.getElapsedTime() - lastMoved).asSeconds() > delay / 20) 
		//or (hardDropReady && hardDrop) //&& (clock.getElapsedTime() - lastMoved).asSeconds() > .0001)
		or ((clock.getElapsedTime() - lastMoved).asSeconds() > delay)) ){
		if (softDrop) { score += 1; }
		else if (hardDrop && hardDropReady) { score += 2; }
		if (!tetrominoes.back().fall(board)) {
			//add to board and add new block
			justHit = true;
			hitTime = clock.getElapsedTime();
			//if (!hardDrop) { hitTime = clock.getElapsedTime(); }
			//else { hitTime = clock.getElapsedTime() - sf::seconds(1); }
		}
		else {
			lastY+=block::size;
			lastMoved = clock.getElapsedTime();
		}
		//tetrominoes.back().rotateLeft();
		//lastMoved = clock.getElapsedTime();
	}

	if (justHit && !couldFall && (clock.getElapsedTime() - hitTime).asSeconds() >= .5) {
		/*printf("adding block to board %d\n", tetrominoes.size());
		for (sf::Vector2i v: tetrominoes.back().getBoardPoints()) {
			printf("%d:%d \n", v.x, v.y);
		}*/
		addToBoard(); //displayBlocks();
		hardDropReady = false;
		if (!lost) { randomAdd(); }
		justHit = false;
	}

	//update ghostPiece
	if (ghostPiece) { delete ghostPiece; }
	int x = globalX + lastX * block::size, y = lastY;
	int num = tetrominoes.back().getType();
	switch (num) {
	case 1:
		y -= block::size;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		y -= block::size / 2;
		x -= block::size / 2;
		break;
	}
	ghostPiece = new tetrominoe(x, y, globalX, globalY, width, height, tetType(num));
	int rotation = tetrominoes.back().getBlocks()[0].getRect().getRotation();

	ghostPiece->setGhost(rotation);
	ghostPiece->updateBoardPoints();
	while (ghostPiece->fall(board)) {}
}

void game::moveRight() {
	if (lost || paused) { return; }
	if (tetrominoes.back().moveRight(board)) { lastX += 1; }
	//std::cout << lastX << std::endl;
}

void game::moveLeft() {
	if (lost || paused) { return; }
	if (tetrominoes.back().moveLeft(board)) { lastX -= 1; }
	//std::cout << lastX << std::endl;
}

void game::rotateRight() {
	if (lost || paused) { return; }
	if (canRotate) { tetrominoes.back().rotateRight(board); }
	//canRotate = false;
}
void game::rotateLeft() {
	if (lost || paused) { return; }
	if (canRotate) { tetrominoes.back().rotateLeft(board); }
	canRotate = false;
}

void game::restoreRotate() {
	canRotate = true;
}

//adds the top tetrominoe to the board
void game::addToBoard() {
	std::vector<int> changedY;
	for (int i = 0; i < tetrominoes.back().getBlocks().size(); ++i) {
		sf::Vector2i v = tetrominoes.back().getBoardPoints()[i];
		if (v.y > 0 && v.x > 0 && board[v.y][v.x]) { lost = true; return; }
		board[v.y][v.x] = &tetrominoes.back().getBlocks()[i];
		if (std::find(changedY.begin(), changedY.end(), v.y) == changedY.end()) {
			changedY.push_back(v.y);
		}
	}

	std::vector<bool> shouldClear;
	for (int y : changedY) {
		bool clearRow = true;
		int count = 0;
		for (int x = 0; x < width; ++x) {
			if (!board[y][x]) { clearRow = false; }
		}
		shouldClear.push_back(clearRow);
	}

	for (int c = 0; c < shouldClear.size(); ++c) {
		if (shouldClear[c]) {
			//printf("clearing %d\n", changedY[c]);
			int i = changedY[c];
			for (int x = 0; x < width; ++x) {
				board[i][x]->drawOff();
			}

			for (int y = 0; y < i; ++y) {
				for (int x = 0; x < width; ++x) {
					if (board[y][x] && board[y][x]->willDraw()) {
						sf::Vector2f v = board[y][x]->getRect().getPosition();
						v.y += block::size;
						board[y][x]->getRect().setPosition(v);
					}
				}
			}
		}
	}

	int count = 0;
	for (int c = 0; c < shouldClear.size(); ++c) {
		if (shouldClear[c]) {
			count += 1;
			cleared += 1;
			int y = changedY[c];
			board.erase(board.begin() + y);
			board.insert(board.begin(), std::vector<block*>(width, NULL));
			for (int i = c; i < shouldClear.size(); ++i) {
				if (changedY[i] < changedY[c]) { changedY[i] += 1;}
			}
		}
	}

	canHold = true;
	level = cleared / 10 + 1;
	
	if (count >= 1) {
		score += (count * 2 - 1) * 100 * level;
		if (count == 4) { score += 100 * level; }
	}

	//displayBlocks();
}

void game::displayBlocks() {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (board[y][x] == NULL) {
				std::cout << "0 ";
			}
			else {
				std::cout << "1 ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void game::hold() {
	if (canHold) {
		justHit = false;

		if (holdIndex == -1) { //hold is empty
			holdIndex = tetrominoes.size()-1;
			randomAdd();
		}
		else {
			/*if (lastX == width) { lastX = width - 1; }
			else if (lastX <= 1) { lastX = 2; };*/
			lastX = width / 2; //set to center
			int x = globalX + lastX * block::size, y = globalY + block::size * 2;
			lastY = y;
			switch (tetrominoes[holdIndex].getType()) {
			case 1:
				y -= block::size;
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				y -= block::size / 2;
				x -= block::size / 2;
				break;
			}

			/*printf("%d : %d \n", x, y);
			for (sf::Vector2i v : tetrominoes[holdIndex].getBoardPoints()) {
				printf("%d:%d \n", v.x, v.y);
			}*/
			tetrominoes[holdIndex].setPosition(x,y); 
			tetrominoes[holdIndex].updateBoardPoints();
			std::iter_swap(tetrominoes.begin() + holdIndex, tetrominoes.begin() + (tetrominoes.size() - 1));
		}

		tetrominoes[holdIndex].setPosition(globalX - block::size * 3.5, globalY + block::size * 2.5);
		canHold = false;
	}
}

void game::pause() {
	paused = true;
	if (musicOn) { music.pause(); }
}

void  game::resume() {
	paused = false;
	if (musicOn) { music.play(); }
}

void game::togglePause() {
	paused = !paused;
	if (musicOn) {
		if (paused) { music.pause(); }
		else { music.play(); }
	}
}
