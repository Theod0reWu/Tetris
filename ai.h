#pragma once

#include "game.h"
#include <queue>
#include <cmath>
#include <algorithm>

void readAiInput(game& aiGame, std::queue<std::string>& aiActions, std::vector<float> vec, bool aiUse = false);
void readAiInput(game& aiGame, std::queue<std::string>& aiActions);

void aiPlayer(game& g, std::queue<std::string>& aiActions);
void aiPlayer(game& g, std::queue<std::string>& aiActions, std::vector<float>& vec);

float boardRating(const std::vector<std::vector <block*> >& board);
float boardRating(const std::vector<std::vector <block*> >& board, std::vector<float> params);

int calculateBest(game& g, std::vector<int> &allNums, std::vector<std::vector<block*> >& currentBoard, int& rots, int& movs, float initRotation = 0, int index = 0);
int calculateBest(game& g, std::vector<int>& allNums, std::vector<std::vector<block*> >& currentBoard, int& rots, int& movs, std::vector<float>& vec, float initRotation = 0, int index = 0);

//https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/
void geneticAlgorithm();
int fitness(std::vector<float>& vec, int games = 50);
void crossover(std::vector<std::vector<float> >& population, std::vector<int>& fitnessValues);

void normalize(std::vector<float>& vec);
float rand_FloatRange(float a, float b);