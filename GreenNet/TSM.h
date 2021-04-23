#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Clustering.h"

class Paths {
public:
	std::vector<std::vector<Local>> PathsDefined;
	void OptimizePath(std::vector<std::vector<Local>> Clusts);
private:
	double CalculateFitness();
	void Mutate(double p_mutation);
	void Selection(int Max_Initial_Pop);
	void Generate(int Max_Pop_Size);
};