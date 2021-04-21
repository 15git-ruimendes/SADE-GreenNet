#pragma once
#include <iostream>
#include <vector>
#include <string>


class Local
{
public:
	long double Type, xX, yY, Stocks, Id;
	Local(std::vector<long double>Loc, const char Ty) {
		if (Ty == 'A') {
			Type = 1;
			Id = Loc[0];
			Stocks = Loc[1];
			xX = Loc[2];
			yY = Loc[3];
		}
		else if (Ty == 'L') {
			Type = 0;
			Id = Loc[0];
			Stocks = Loc[3];
			xX = Loc[1];
			yY = Loc[2];
		}
	};
};


class Clusters 
{
public:
	std::vector<std::vector<Local>> ClusterBest;
	std::vector<std::vector<Local>> ClusterCurrent;
	void OptimizeClusters(std::vector<Local> Armazens, std::vector<Local> Lojas);
	void printClusterCurrent();
private:
	void CreatFirstSolution(std::vector<Local> Armazens, std::vector<Local> Lojas);
	double EvaluateSolution();
	double DetermineDistance(long double x1, long double x2, long double y1, long double y2);
};




