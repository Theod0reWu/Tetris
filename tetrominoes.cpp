#include "tetrominoes.h"
#include <stdio.h>

tetrominoe::tetrominoe(float x, float y, int gx, int gy, int w, int h,
	tetType tetrominoeType, sf::Texture& texture) {

	type = tetrominoeType;
	blocks = std::vector<block>();
	setBlocks(x,y);

	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i].setTexture(&texture);
	}

	globalX = gx; globalY = gy;
	width = w; height = h;

	updateBoardPoints();
}

//used for ghost block creation, lol can't simplify for some reason
tetrominoe::tetrominoe(float x, float y, int gx, int gy, int w, int h, tetType tetrominoeType) {
	type = tetrominoeType;
	blocks = std::vector<block>();
	setBlocks(x,y);

	sf::Color outlineColor;
	switch (tetrominoeType) {
	case tetType::I:
		outlineColor = sf::Color::Cyan;
		break;
	case tetType::O:
		outlineColor = sf::Color::Yellow;
		break;
	case tetType::T:
		outlineColor = sf::Color::Magenta;
		break;
	case tetType::S:
		outlineColor = sf::Color::Green;
		break;
	case tetType::Z:
		outlineColor = sf::Color::Red;
		break;
	case tetType::L:
		outlineColor = sf::Color(255, 128, 0);
		break;
	case tetType::J:
		//outlineColor = sf::Color::Blue; //too dark
		outlineColor = sf::Color(0, 100, 255);
		break;
	}

	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i].getRect().setOutlineColor(outlineColor);
	}

	globalX = gx; globalY = gy;
	width = w; height = h;

	updateBoardPoints();
}

//tetrominoe::tetrominoe(const tetrominoe& tet) {
//	
//}

void tetrominoe::setBlocks(int x, int y) {
	switch (type) {
	case tetType::I:
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size*2, block::size);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size, block::size);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(0,block::size);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size*-1, block::size);
		break;
	case tetType::O:
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size, block::size);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(0, block::size);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size, 0);
		blocks.push_back(block(x, y));
		break;
	case tetType::T:
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * 1.5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * 1.5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * -.5, block::size * .5);
		break;
	case tetType::S:
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * 1.5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * 1.5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * -.5, block::size * 1.5);
		break;
	case tetType::Z:
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * 1.5, block::size * 1.5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * 1.5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * -.5, block::size * .5);
		break;
	case tetType::L:
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * 1.5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * -.5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * -.5, block::size * 1.5);
		break;
	case tetType::J:
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * 1.5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * .5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * -.5, block::size * .5);
		blocks.push_back(block(x, y));
		blocks.back().setOrigin(block::size * 1.5, block::size * 1.5);
		break;
	}
}

void tetrominoe::rotateLeft(std::vector<std::vector <block*> > &board) {
	for (int i = 0; i < blocks.size(); ++i) { blocks[i].rotateLeft(); }
	updateBoardPoints();
	bool worked = true;
	for (int i = 0; i < boardPoints.size(); ++i) {
		if (board[boardPoints[i].y][boardPoints[i].x]) { worked = false; break; }
	}
	if (!worked) { //undo it
		for (int i = 0; i < blocks.size(); ++i) { blocks[i].rotateRight(); }
		updateBoardPoints();
	}
}

void tetrominoe::rotateRight(std::vector<std::vector <block*> > &board) {
	for (int i = 0; i < blocks.size(); ++i) { blocks[i].rotateRight(); }
	updateBoardPoints();
	bool worked = true;
	for (int i = 0; i < boardPoints.size(); ++i) {
		if (boardPoints[i].x >= width || boardPoints[i].x < 0
			|| boardPoints[i].y >= height || boardPoints[i].y < 0
			|| board[boardPoints[i].y][boardPoints[i].x]) { worked = false; break; }
	}
	if (!worked) { //undo it
		for (int i = 0; i < blocks.size(); ++i) { blocks[i].rotateLeft(); }
		updateBoardPoints();
	}
}

void tetrominoe::rotateRight() {
	for (int i = 0; i < blocks.size(); ++i) { blocks[i].rotateRight(); }
	updateBoardPoints();
}

bool tetrominoe::canFall(std::vector<std::vector <block*> > &board) {
	bool can = true;
	//for (int i = 0; i < boardPoints.size() && can; i++){ printf("%d:%d \n", boardPoints[i].x, boardPoints[i].y); }
	//printf("%f \n",blocks[0].getRect().getRotation() );
	for (int i = 0; i < boardPoints.size() && can; i++)
	{
		if (boardPoints[i].y >= height - 1 || (boardPoints[i].y >= -1 && board[boardPoints[i].y+1][boardPoints[i].x])) { can = false; }
	}
	return can;
}

bool tetrominoe::fall(std::vector<std::vector <block*> > &board) {
	if (!canFall(board)) { return false; }
	for (int i = 0; i < blocks.size(); ++i) {
		sf::Vector2f v = blocks[i].getRect().getPosition();
		v.y += block::size;
		blocks[i].getRect().setPosition(v);
	}
	updateBoardPoints();
	return true;
}

bool tetrominoe::canMoveRight(std::vector<std::vector <block*> > &board) {
	bool can = true;
	for (int i = 0; i < boardPoints.size(); i++)
	{
		//printf("%f:%f \n", boardPoints[i].x, boardPoints[i].y);
		if (boardPoints[i].x >= width - 1 || (boardPoints[i].x < height -1 && board[boardPoints[i].y][boardPoints[i].x + 1])) { can = false; }
	}
	return can;
}
bool tetrominoe::canMoveLeft(std::vector<std::vector <block*> > &board) {
	bool can = true;
	for (int i = 0; i < boardPoints.size(); i++)
	{
		//printf("%f:%f \n", boardPoints[i].x, boardPoints[i].y);
		if (boardPoints[i].x <= 0 || (boardPoints[i].x > 0 && board[boardPoints[i].y][boardPoints[i].x - 1])) { can = false; }
	}
	return can;
}

bool tetrominoe::moveRight(std::vector<std::vector <block*> > &board) {
	if (!canMoveRight(board)) { return false; }
	for (int i = 0; i < blocks.size(); ++i) {
		sf::Vector2f v = blocks[i].getRect().getPosition();
		v.x += block::size;
		blocks[i].getRect().setPosition(v);
	}
	updateBoardPoints();
	return true;
}

bool tetrominoe::moveLeft(std::vector<std::vector <block*> > &board) {
	if (!canMoveLeft(board)) { return false; }
	for (int i = 0; i < blocks.size(); ++i) {
		sf::Vector2f v = blocks[i].getRect().getPosition();
		v.x -= block::size;
		blocks[i].getRect().setPosition(v);
	}
	updateBoardPoints();
	return true;
}

void tetrominoe::updateBoardPoints() {
	boardPoints.clear();
	for (int i = 0; i < blocks.size(); ++i) {
		boardPoints.push_back(blocks[i].getBoardPoint(globalX, globalY));
	}
}

std::vector<sf::Vector2i>& tetrominoe::getBoardPoints() {
	return boardPoints;
}