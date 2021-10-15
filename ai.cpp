#include "ai.h"

void aiPlayer(game& g, std::queue<std::string>& aiActions) {
	std::vector<float> temp;
	aiPlayer(g, aiActions, temp);
}

void aiPlayer(game& g, std::queue<std::string>& aiActions, std::vector<float>& vec) {
	std::vector<std::vector<block*> > currentBoard;
	for (int y = 0; y < g.getBoardState().size(); ++y) {
		currentBoard.push_back(std::vector<block*>(g.getBoardState()[0].size(), NULL));
		for (int x = 0; x < g.getBoardState()[0].size(); ++x) {
			if (g.getBoardState()[y][x]) { currentBoard[y][x] = g.getBoardState()[y][x]; }
		}
	}

	//create tetrominoe procedure
	int num = g.getActive();
	int initRotation = g.getActiveRotation();
	std::vector<int> allNums;
	allNums.push_back(num);
	//allNums.push_back(g.getNextTets()[0]);
	//allNums.push_back(g.getNextTets()[1]);
	//allNums.push_back(g.getNextTets()[2]);

	int numMoves = 0; int numRotations = 0;
	if (vec.size() != 6) {
		calculateBest(g, allNums, currentBoard, numRotations, numMoves, initRotation);
	}
	else {
		calculateBest(g, allNums, currentBoard, numRotations, numMoves, vec, initRotation);
	}
	//printf("%d,%d\n", numRotations, numMoves);
	for (int i = 0; i < numRotations; i++) {
		aiActions.push("rotR");
	}
	if (numMoves < 0) {
		for (int i = 0; i < numMoves * -1; i++) {
			aiActions.push("moveL");
		}
	}
	else if (numMoves > 0) {
		for (int i = 0; i < numMoves; i++) {
			aiActions.push("moveR");
		}
	}

	aiActions.push("dropHon");
	aiActions.push("dropHoff");
	//printf("%d, %d \n", numRotations, numMoves);
}


int calculateBest(game& g, std::vector<int>& allNums, std::vector<std::vector<block*> >& currentBoard, int& rots, int& movs, float initRotation, int index) {
	std::vector<float> temp;
	return calculateBest(g, allNums, currentBoard, rots, movs, temp, initRotation, index);
}

 int calculateBest(game& g, std::vector<int>& allNums, std::vector<std::vector<block*> >& currentBoard, int & rots, int & movs, std::vector<float>& vec, float initRotation, int index){
	//printf("calculating %d...\n", index);
	 //printf("%d\n", index);
	 int num = allNums[index];
	 while (initRotation < 0) { initRotation += 360; }
	int x = g.getGlobalX()+ g.getLastX() * block::size, y = g.getLastY();
	if (index != 0) { 
		x = g.getGlobalX() + g.getWidth() / 2 * block::size;
		y = g.getGlobalY() + block::size * 2;
	}
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

	std::vector <std::vector<float> > rightScores;
	std::vector <std::vector<float> > leftScores;
	int maxRotate = 4;
	if (num == 0 || num == 3 || num == 4) { maxRotate = 2; }
	else if (num == 1) { maxRotate = 1; }

	//std::vector<int> rotatesToCheck
	for (int rotateTimes = 0; rotateTimes < maxRotate; ++rotateTimes) {

		tetrominoe countTet(x, y, g.getGlobalX(), g.getGlobalY(), g.getWidth(), g.getHeight(), tetType(num));
		for (int i = 0; i < initRotation; i+=90) { countTet.rotateRight(); }

		for (int i = 0; i < rotateTimes; ++i) {countTet.rotateRight(currentBoard);}
		/*for (sf::Vector2i v : tempTet.getBoardPoints()) {
			printf("%d:%d \n", v.x, v.y);
		}*/

		int numRight = 0;
		while (countTet.moveRight(currentBoard)) { numRight++; }
		//printf("right moves:%d\n", numRight);

		int numLeft = 0;
		while (countTet.moveLeft(currentBoard)) { numLeft++; }
		numLeft -= numRight;
		//printf("left moves:%d\n", numLeft);

		std::vector<float> rightMove(numRight+1, 0);
		std::vector<float> leftMove(numLeft+1, 0);

		for (int r = 0; r < numRight+1; ++r) {
			tetrominoe tempTet(x, y, g.getGlobalX(), g.getGlobalY(), g.getWidth(), g.getHeight(), tetType(num));
			for (int i = 0; i < initRotation; i += 90) { tempTet.rotateRight(); }
			for (int i = 0; i < rotateTimes; ++i) { tempTet.rotateRight(currentBoard); }

			for (int i = 0; i < r; ++i) { tempTet.moveRight(currentBoard); }
			while (tempTet.fall(currentBoard)) {}
			for (int i = 0; i < tempTet.getBoardPoints().size(); i++) {
				currentBoard[tempTet.getBoardPoints()[i].y][tempTet.getBoardPoints()[i].x] = &tempTet.getBlocks()[0];
			}
			if (vec.size() != 6) {
				rightMove[r] = boardRating(currentBoard);
			}
			else {
				rightMove[r] = boardRating(currentBoard, vec);
			}
			if (index + 1 < allNums.size()) {
				int garbage;
				//printf("%d calling %d \n", index, index + 1);
				rightMove[r] += calculateBest(g, allNums, currentBoard, garbage, garbage, 0, index + 1);
			}

			//printf("rot: %d, move:%d, score: %d \n", rotateTimes, r, rightMove[r]);
			for (int i = 0; i < tempTet.getBoardPoints().size(); i++) {
				currentBoard[tempTet.getBoardPoints()[i].y][tempTet.getBoardPoints()[i].x] = NULL;
			}
		}

		//printf("%d done with rightside \n", index);

		for (int l = 1; l < numLeft+1; ++l) {
			tetrominoe tempTet(x, y, g.getGlobalX(), g.getGlobalY(), g.getWidth(), g.getHeight(), tetType(num));
			for (int i = 0; i < initRotation; i += 90) { tempTet.rotateRight(); }
			for (int i = 0; i < rotateTimes; ++i) { tempTet.rotateRight(currentBoard); }

			for (int i = 0; i < l; ++i) { tempTet.moveLeft(currentBoard); }
			while (tempTet.fall(currentBoard)) {}
			for (int i = 0; i < tempTet.getBoardPoints().size(); i++) {
				currentBoard[tempTet.getBoardPoints()[i].y][tempTet.getBoardPoints()[i].x] = &tempTet.getBlocks()[0];
			}
			if (vec.size() != 6) {
				leftMove[l] = boardRating(currentBoard);
			}
			else {
				leftMove[l] = boardRating(currentBoard, vec);
			}
			if (index + 1 < allNums.size()) {
				int garbage;
				leftMove[l] += calculateBest(g, allNums, currentBoard, garbage, garbage, 0, index + 1);
			}
			//printf("rot: %d, move:%d, score: %d \n", rotateTimes, l*-1, leftMove[l]);
			for (int i = 0; i < tempTet.getBoardPoints().size(); i++) {
				currentBoard[tempTet.getBoardPoints()[i].y][tempTet.getBoardPoints()[i].x] = NULL;
			}
		}

		rightScores.push_back(rightMove);
		leftScores.push_back(leftMove);
	}
	float maxScore = rightScores[0][0];
	int numMoves = 0; int numRotations = 0;
	for (int rotation = 0; rotation < leftScores.size(); rotation++) {
		for (int move = 1; move < leftScores[rotation].size(); move++) {
			if (leftScores[rotation][move] > maxScore) {
				numMoves = -1*move; numRotations = rotation;
				maxScore = leftScores[rotation][move];
				//printf("rot: %d, move:%d, score: %d \n", rotation, move, leftScores[rotation][move]);
			}
		}
	}

	for (int rotation = 0; rotation < rightScores.size(); rotation++) {
		for (int move = 0; move < rightScores[rotation].size(); move++) {
			if (rightScores[rotation][move] > maxScore) {
				numMoves = move; numRotations = rotation;
				maxScore = rightScores[rotation][move];
				//printf("rot: %d, move:%d, score: %d \n", rotation, move, rightScores[rotation][move]);
			}
		}
	}

	rots = numRotations;
	movs = numMoves;

	return maxScore;
}

float boardRating(const std::vector<std::vector <block*> >& board) {
	std::vector<float> p = {.76067,  -.3566 , 0, -.51 , -.18448 , 0};
	//std::vector<float> p = { .5217,  .47222 , .1972, -.5274 , -.207 , -.38 };
	// 0.611293:-0.615214:-0.125092:-0.385032:-0.219618:-0.188954 10000
	//0.326177:-0.677579:-0.155139:-0.596212:-0.222683:-0.073288
	//0.495084:-0.567507:-0.154598:-0.563029:-0.241405:-0.183438
	p = { 0.381353,-0.683406 , -0.095332 , -0.555439 , -0.239082 , -0.112986 };
	return boardRating(board, p);
}

float boardRating(const std::vector<std::vector <block*> >& board, std::vector<float> params) {

	int completeLines = 0;
	for (std::vector<block*> v : board) {
		bool complete = true;
		for (block* p : v) {
			if (!p) { complete = false; }
		}
		if (complete) { completeLines += 1; }
	}

	int holes = 0; int maxHeight = 0; int aggregateHeight = 0;
	int coveredAmount = 0;
	std::vector<int> heights;
	for (int col = 0; col < board[0].size(); ++col) {
		bool start = false;
		for (int row = 0; row < board.size(); ++row) {
			if (!board[row][col] && start) { holes++; coveredAmount += heights.back() - board.size() + row;}
			else if (board[row][col] && !start) { 
				start = true; 
				heights.push_back(board.size() - row);
				aggregateHeight += heights.back();
				if (heights.back() > maxHeight) { maxHeight = heights.back(); }
			}
		}
		if (!start) { heights.push_back(0); }
	}

	int bumpiness = 0;
	for (int h = 1; h < heights.size(); ++h) {
		bumpiness += std::abs(heights[h] - heights[h - 1]);
	}

	/*printf("%d\n",bumpiness);
	for (int y = 0; y < board.size(); ++y) {	
		for (int x = 0; x < board[0].size(); ++x) {
			if (board[y][x] == NULL) {
				printf( "0 ");
			}
			else {
				printf( "1 ");
			}
		}
		printf("\n");
	}
	printf("\n");*/
	
	return completeLines * params[0] + holes * params[1] + maxHeight * params[2] + aggregateHeight * params[3] + bumpiness * params[4] + coveredAmount * params[5];
}

void readAiInput(game& aiGame, std::queue<std::string>& aiActions) {
	std::vector<float> temp;
	readAiInput(aiGame, aiActions, temp);
}

void readAiInput(game& aiGame, std::queue<std::string>& aiActions, std::vector<float> vec, bool aiUse) {
	bool hardDropIt = true;
	if (aiGame.lostYet()) {}
	else if (aiActions.size() == 0) {
		//if (aiUse) { sf::Clock clock; aiGame.update(clock); }
		aiPlayer(aiGame, aiActions, vec);
	}
	else {
		if (!hardDropIt) {
			aiGame.softDropOn();
		}
		if (aiActions.front() == "rotR") {
			aiGame.rotateRight();
			//printf("rotatingRight\n");
		}
		else if (aiActions.front() == "moveR") {
			aiGame.moveRight();
			//printf("movingRight\n");
		}
		else if (aiActions.front() == "moveL") {
			aiGame.moveLeft();
			//printf("movingLeft\n");
		}
		else if (aiActions.front() == "dropHon" && hardDropIt) {
			aiGame.hardDropOn();
			//printf("harddropping \n");
			if (aiUse) { sf::Clock clock; aiGame.update(clock); }
		}
		else if (aiActions.front() == "dropHoff" && hardDropIt) {
			aiGame.readyHardDrop();
			aiGame.hardDropOff();
			//printf("harddropping off \n");
			if (aiUse) { sf::Clock clock; aiGame.update(clock); }
		}
		aiActions.pop();
	}
}

void geneticAlgorithm() {
	printf("starting\n");
	int generations = 10;

	const size_t popSize = 200; //must be at least 20
	std::vector<std::vector<float> > population;
	std::vector<int> fitnessValues(popSize, -1);

	//population initialization
	for (size_t i = 0; i < popSize; i++)
	{
		std::vector<float> temp;
		temp.push_back(rand_FloatRange(.1, 1));
		for (size_t x = 1; x < 6; ++x) { temp.push_back(rand_FloatRange(-1 , 0)); }
		normalize(temp);
		population.push_back(temp);
	}
	printf("population initialized\n");

	for (int gen = 0; gen < generations; ++gen) {
		printf("starting gen: %d\n", gen);
		//fitness function calculation
		for (size_t i = 0; i < popSize; i++)
		{
			if (fitnessValues[i] == -1) {
				fitnessValues[i] = fitness(population[i]);
				printf("Fitness for %d -> %f:%f:%f:%f:%f:%f calculated: %d\n", i, 
					population[i][0], population[i][1], population[i][2], population[i][3], population[i][4], population[i][5],
					fitnessValues[i]);
			}
		}

		if (gen != generations - 1) {
			//crossover && mutation -> survivor selections
			crossover(population, fitnessValues);
			printf("crossed over\n");
		}
	}

	//terminate and return best
	int maxIndex = 0;
	for (int i = 1; i < fitnessValues.size(); ++i) {
		if (fitnessValues[i] > fitnessValues[maxIndex]) { maxIndex = i; }
	}

	printf("fitness: %d\n", fitnessValues[maxIndex]);
	for (int i = 0; i < population[0].size(); ++i) {
		printf("%d -> %f \n", i, population[maxIndex][i]);
	}
}

void crossover(std::vector<std::vector<float> >& population, std::vector<int>& fitnessValues) {
	std::vector<std::vector<float> > offspring;

	while (offspring.size() < population.size() * .3) {
		//tournament selection
		std::vector<int> selection;
		while (selection.size() < population.size() / 10) {
			//printf("%d\n", selection.size());
			int index = rand() % population.size();
			if (std::find(selection.begin(), selection.end(), index) == selection.end()) {
				selection.push_back(index);
			}
		}
		int best, secBest;
		if (fitnessValues[selection[0]] > fitnessValues[selection[1]]) {best = 0; secBest = 1;}
		else { best = 1; secBest = 0; }

		for (int i = 2; i < selection.size(); ++i) {
			if (fitnessValues[selection[i]] > fitnessValues[selection[best]]) {
				secBest = best; best = i;
			}
		}
		
		best = selection[best]; secBest = selection[secBest];

		//crossover the two parents
		std::vector<float> child;
		for (size_t i = 0; i < population[0].size(); i++)
		{
			child.push_back(population[best][i] * fitnessValues[best] + population[secBest][i] * fitnessValues[secBest]);
		}
		normalize(child);

		//mutation of the child
		if (rand() % 100 <= 5) {
			child[rand() % child.size()] += rand_FloatRange(-.2, .2);
			normalize(child);
		}
		offspring.push_back(child);
	}
	printf("gotten offspring %d\n", offspring.size());

	//survivor selections
	std::vector<int> killIndices;
	while (killIndices.size() < offspring.size()) {
		int minIndex = 0;
		for (int i = 0; i < fitnessValues.size(); ++i) {
			if (std::find(killIndices.begin(), killIndices.end(), i) == killIndices.end()) {
				minIndex = i; break;
			}
		}

		for (int i = 0; i < fitnessValues.size(); ++i) {
			if (fitnessValues[i] <= fitnessValues[minIndex] && std::find(killIndices.begin(), killIndices.end(), i) == killIndices.end()) {
				minIndex = i;
			}
		}
		killIndices.push_back(minIndex);
	}

	for (int i = 0; i < killIndices.size(); i++)
	{
		//printf("killing %d\n", killIndices[i]);
		population[ killIndices[i] ] = offspring[i];
		fitnessValues[killIndices[i]] = -1;
	}
}

int fitness(std::vector<float>& vec, int games) {
	int fit = 0;
	for (int i = 0; i < games; ++i) {
		game aiGame(0, 0, false);
		sf::Clock clock;
		std::queue<std::string> aiActions;
		while (!aiGame.lostYet() && aiGame.getLinesCleared() < 1500) {
			readAiInput(aiGame, aiActions, vec, true);
			//aiGame.update(clock);
		}
		fit += aiGame.getLinesCleared();
	}

	return fit;
}

float rand_FloatRange(float a, float b)
{
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

void normalize(std::vector<float>& vec) {
	float mag = 0;
	for (size_t i = 0; i < vec.size(); i++)
	{
		mag += vec[i] * vec[i];
	}

	mag = std::sqrt(mag);

	for (size_t i = 0; i < vec.size(); i++)
	{
		vec[i] /= mag;
	}
}