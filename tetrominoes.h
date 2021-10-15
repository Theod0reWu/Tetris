#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <cmath>

enum class tetType {
	I, //cyan |
	O, //yellow []
	T, //purple T
	S, //green
	Z, //red
	J, //blue
	L //orange
};

class block {
public:
	static const int size = 50; //pixels for width/height
	block(float x = 0, float y = 0) {
		rect.setSize(sf::Vector2f(size, size));
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1);
		rect.setPosition(x, y);

		//rect.setFillColor(sf::Color::Black);
		shouldDraw = true;
	}

	void drawOff() {
		shouldDraw = false;
	}

	bool willDraw() {
		return shouldDraw;
	}

	bool draw(sf::RenderWindow& window) const {
		if (shouldDraw) { window.draw(rect);}
		return shouldDraw;
	}
	void setTexture(const sf::Texture* texture) {
		rect.setTexture(texture);
	}
	void setColor(sf::Color c) {
		color = c;
	}
	void setOrigin(float x, float y) {
		rect.setOrigin(x, y);
	}

	void rotateRight() {
		rect.rotate(90);
	}
	void rotateLeft() {
		rect.rotate(-90);
	}

	void setPosition(float x, float y) {
		rect.setPosition(x, y);
	}

	sf::RectangleShape& getRect() {
		return rect;
	}

	sf::Vector2i getBoardPoint(int x, int y) {
		sf::FloatRect r = rect.getGlobalBounds();
		sf::Vector2f vec = sf::Vector2f(r.left,r.top);
		//printf("global block= %f:%f\n", vec.x, vec.y);
		vec.x -= x; vec.y -= y;
		vec.x /= size; vec.y /= size;
		//printf("board block= %f:%f\n", vec.x, vec.y);
		return sf::Vector2i(round(vec.x),round(vec.y));
	}

	//~block() {}

private:
	sf::Texture text;
	sf::Color color;

	sf::RectangleShape rect;
	bool shouldDraw;
};

class tetrominoe {
public:
	tetrominoe(float x, float y, int gx, int gy, int w, int h, tetType tetrominoeType);
	tetrominoe(float x, float y, int gx, int gy, int w, int h, tetType tetrominoeType, sf::Texture& texture);
	//tetrominoe(const tetrominoe& tet);

	std::vector<block>& getBlocks() {
		return blocks;
	}

	bool draw(sf::RenderWindow& window) {
		bool didDraw = false;
		for (int i = 0; i < blocks.size(); ++i) {
			if (blocks[i].draw(window)) { didDraw = true; }
		}
		return didDraw;
	}

	void setPosition(float x, float y) {
		for (int i = 0; i < blocks.size(); ++i) { blocks[i].setPosition(x, y); }
	}

	void setGhost(int deg) {
		for (int i = 0; i < blocks.size(); ++i) { 
			blocks[i].getRect().setFillColor(sf::Color::Black);
			blocks[i].getRect().setRotation(deg);
		}
	}

	//bool canRotate(std::vector<std::vector <block*> >& board);
	void rotateRight(std::vector<std::vector <block*> > &board);
	void rotateLeft(std::vector<std::vector <block*> > &board);
	bool canFall(std::vector<std::vector <block*> > &board);
	bool fall(std::vector<std::vector <block*> > &board);

	void rotateRight();

	bool canMoveRight(std::vector<std::vector <block*> > &board);
	bool canMoveLeft(std::vector<std::vector <block*> > &board);
	bool moveRight(std::vector<std::vector <block*> > &board);
	bool moveLeft(std::vector<std::vector <block*> > &board);

	std::vector<sf::Vector2i>& getBoardPoints();
	void updateBoardPoints();

	int getType() {
		return (int)type;
	}
private:
	void setBlocks(int x, int y);

	tetType type;
	std::vector<block> blocks;

	std::vector<sf::Vector2i> boardPoints;

	int globalX, globalY;
	int width, height; //of the play area
};