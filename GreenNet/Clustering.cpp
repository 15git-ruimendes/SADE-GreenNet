#include "Clustering.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#define TempValD 0.5
#define TempValF 0.5


void Clusters::OptimizeClusters(std::vector<Local> Locais)
{

}

void Clusters::CreatFirstSolution(std::vector<Local> Locais)
{
	
}

double Clusters::EvaluateSolution()
{
	long double MaxDistance = 0, FullFilment = 0, auxDist = 0, auxFull = 0, Deliv = 0;
	long double x1, y1;

	for (int i = 0; i < sizeof(ClusterCurrent)/4;i++) {
		x1 = ClusterCurrent[i][0].xX;
		y1 = ClusterCurrent[i][0].yY;
		for (int j = 1; j < 10;j++) {
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

