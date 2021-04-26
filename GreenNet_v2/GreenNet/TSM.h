#pragma once
#include "Clustering.h"
#include <algorithm>

#define MaxPopSize 100
#define InitPopSize 20

class Paths {
public:
	std::vector<std::vector<Local>> PathsDefined;
	std::vector<std::vector<Local>> Pop;
	std::vector<std::vector<Local>> InitPop;
	void OptimizePath(std::vector<std::vector<Local>> Clusts);
private:
	double CalculateFitnessAndGenerateNew();
	double CalculateFitness(std::vector<Local> Prop);
	void Mutate();
	void Generate();
	void DevideNSwitch(std::vector<Local> Mother, std::vector<Local> Father);
	void GenerateFirstPop(std::vector<Local> Clust);
	double DetermineDistance(long double x1, long double x2, long double y1, long double y2);
};