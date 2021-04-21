#include "Clustering.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#define TempValD 0.5
#define TempValF 0.5


void Clusters::OptimizeClusters(std::vector<Local> Armazens, std::vector<Local> Lojas)
{
	CreatFirstSolution(Armazens, Lojas);
	std::cout << EvaluateSolution()<<std::endl;
}

void Clusters::printClusterCurrent()
{
	std::cout << "Clusters Being Printed! \n";
	for (int i = 0; i < ClusterCurrent.size(); i++) {
		for (int j = 0; j < ClusterCurrent[i].size(); j++) {
			std::cout << ClusterCurrent[i][j].Id << "\t";
		}
		std::cout << std::endl;
	}
}

void Clusters::CreatFirstSolution(std::vector<Local> Armazens, std::vector<Local> Lojas)
{
	long double xXArm, yYArm, Sum = 0, MinTempDist = 1000000, TempDist;
	int IdMin = 0;
	for (int i = 0; i < Armazens.size(); i++) {
		xXArm = Armazens[i].xX;
		yYArm = Armazens[i].xX;
		ClusterCurrent.resize(i+1);
		ClusterCurrent[i].push_back(Armazens[i]);
		if (Lojas.size() != 0) {
			while (Armazens[i].Stocks >= Sum) {
				for (int j = 0; j < Lojas.size(); j++) {
					TempDist = DetermineDistance(xXArm, Lojas[j].xX, yYArm, Lojas[j].yY);
					if (TempDist <= MinTempDist) {
						MinTempDist = TempDist;
						IdMin = j;
					}
				}
				
				MinTempDist = 10000000;
				if (Lojas.size() == 0) {
					break;
				}
				Sum += Lojas[IdMin].Stocks;

				if (Sum >= Armazens[i].Stocks) {
					Sum = 0;
					break;
				}
				ClusterCurrent[i].push_back(Lojas[IdMin]);
				Lojas.erase(Lojas.begin() + (IdMin));
			}
		}
	}
}

double Clusters::EvaluateSolution()
{
	long double MaxDistance = 0, FullFilment = 0, auxDist = 0, auxFull = 0, Deliv = 0;
	long double x1, y1;

	for (int i = 0; i < ClusterCurrent.size();i++) {
		x1 = ClusterCurrent[i][0].xX;
		y1 = ClusterCurrent[i][0].yY;
		for (int j = 1; j < ClusterCurrent[i].size();j++) {
			FullFilment += ClusterCurrent[i][j].Stocks;
			if (MaxDistance < DetermineDistance(x1, ClusterCurrent[i][j].xX, y1, ClusterCurrent[i][j].yY))
				MaxDistance = DetermineDistance(x1, ClusterCurrent[i][j].xX, y1, ClusterCurrent[i][j].yY);
		}
		auxDist+=MaxDistance;
		auxFull += (ClusterCurrent[i][0].Stocks - FullFilment);
		MaxDistance = 0;
		FullFilment = 0;
	}

	return TempValD*auxDist + TempValF*auxFull;
}

double Clusters::DetermineDistance(long double x1, long double x2, long double y1, long double y2)
{
	return sqrt(pow((x2-x1),2)+pow((y2-y1),2));
}

