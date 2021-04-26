#include "Clustering.h"

#define TempValD 2
#define TempValF 0.1


void Clusters::OptimizeClusters(std::vector<Local> Armazens, std::vector<Local> Lojas, float sigma)
{
	clock_t init;
	double diff = 0,rho,Temp;
	srand((unsigned)std::time(0));

	init = clock();
	CreatFirstSolution(Armazens, Lojas);
	double BestVal = EvaluateSolution('C');
	std::cout << "Valor de Avaliacao:   ";
	std::cout << BestVal << std::endl;
	printClusterCurrent('C');
	printf("=================================== %d\n", ClusterCurrent.size());
	ClusterBest = ClusterCurrent;
	while ((clock() - init)/CLOCKS_PER_SEC < 150 ) {
		if (RemaningLojas.Id == 0)
		{
			CreatNeighborhood();
			RemaningLojas.Id = 0;
		}
		else {
			CreatNeighborhood();
		}

		Temp = exp(-sigma * (clock() - init));

		if (EvaluateSolution('P') < BestVal) {
			BestVal = EvaluateSolution('P');
			ClusterBest = ProposedCluster;
			
			std::cout << "Valor de Avaliacao:   ";
			std::cout << BestVal << std::endl;
			printClusterCurrent('B');
			printf("=================================== %d\n",ClusterBest.size());

		}
		
		diff = EvaluateSolution('P') - EvaluateSolution('C');
		rho = exp(-diff / Temp);
		if (rand() % 1 < rho)
		{
			ClusterCurrent = ProposedCluster;
			ProposedCluster.clear();
		}
		else 
			ProposedCluster.clear();
	}
}

void Clusters::printClusterCurrent(char Qual)
{
	if (Qual == 'C') 
	{
		std::cout << "ClusterCurrent Being Printed! \n";
		for (int i = 0; i < ClusterCurrent.size(); i++) {
			for (int j = 0; j < ClusterCurrent[i].size(); j++) {
				std::cout << ClusterCurrent[i][j].Id << "\t";
			}
			std::cout << std::endl;
		}
	}
	else if (Qual == 'P') 
	{
		std::cout << "ProposedCluster Being Printed! \n";
		for (int i = 0; i < ProposedCluster.size(); i++) {
			for (int j = 0; j < ProposedCluster[i].size(); j++) {
				std::cout << ProposedCluster[i][j].Id << "\t";
			}
			std::cout << std::endl;
		}
	}
	else if (Qual == 'B') 
	{
		std::cout << "ClusterBest Being Printed! \n";
		for (int i = 0; i < ClusterBest.size(); i++) {
			for (int j = 0; j < ClusterBest[i].size(); j++) {
				std::cout << ClusterBest[i][j].Id << "\t";
			}
			std::cout << std::endl;
		}
	}
}

void Clusters::CreatNeighborhood()
{
	int i1 = 0, i2 = 0, i3 = 0, i4 = 0, j1 = 0, j2 = 0, j3 = 0, j4 = 0, loja1 = 0;
	do {
		srand((unsigned)std::time(0));
		i1 = rand() % (ClusterCurrent.size());
		i2 = rand() % (ClusterCurrent.size());
		i3 = rand() % (ClusterCurrent.size());


		if ((ClusterCurrent[i1].size() - 1 > 0) && (ClusterCurrent[i2].size() - 1 > 0) && (ClusterCurrent[i3].size() - 1 > 0))
		{

			j1 = rand() % (ClusterCurrent[i1].size() - 1) + 1;
			j2 = rand() % (ClusterCurrent[i2].size() - 1) + 1;
			j3 = rand() % (ClusterCurrent[i3].size() - 1) + 1;
		}
		else {
			j1 = 0;
			j2 = 0;
			j3 = 0;
		}
	} while (i1 == i2 || i1 == i3 || i2 == i3);
	ProposedCluster = ClusterCurrent;
	if (j1 == j2 && j2 == j3 && j3 == 0)
		return;

	if ((int)RemaningLojas.Id == 0)
	{
		ProposedCluster[i2].push_back(ProposedCluster[i1][j1]);
		ProposedCluster[i1].push_back(ProposedCluster[i2][j2]);
		RemaningLojas = (ProposedCluster[i3][j3]);
		ProposedCluster[i1].erase(ProposedCluster[i1].begin() + j1);
		ProposedCluster[i2].erase(ProposedCluster[i2].begin() + j2);
		ProposedCluster[i3].erase(ProposedCluster[i3].begin() + j3);
	}
	else if ((int)RemaningLojas.Id != 0)
	{
		ProposedCluster[i3].push_back(RemaningLojas);
	}
	return;
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

double Clusters::EvaluateSolution(char Qual)
{
	long double MaxDistance = 0, FullFilment = 0, auxDist = 0, auxFull = 0, Deliv = 0;
	long double x1, y1;
	if (Qual == 'C') {
		for (int i = 0; i < ClusterCurrent.size();i++) {
			x1 = ClusterCurrent[i][0].xX;
			y1 = ClusterCurrent[i][0].yY;
			for (int j = 1; j < ClusterCurrent[i].size();j++) {
				FullFilment += ClusterCurrent[i][j].Stocks;
				if (MaxDistance < DetermineDistance(x1, ClusterCurrent[i][j].xX, y1, ClusterCurrent[i][j].yY))
					MaxDistance = DetermineDistance(x1, ClusterCurrent[i][j].xX, y1, ClusterCurrent[i][j].yY);
			}
			auxDist += MaxDistance;
			if ((ClusterCurrent[i][0].Stocks - FullFilment) < 0)
				return 60000;
			else 
				auxFull += (ClusterCurrent[i][0].Stocks - FullFilment)/ ClusterCurrent[i][0].Stocks;
			MaxDistance = 0;
			FullFilment = 0;
			Deliv += ClusterCurrent[i].size();
		}

		return (TempValD) * auxDist/(60 * ClusterCurrent.size()) + TempValF * auxFull;
	}
	if (Qual == 'P') {
		for (int i = 0; i < ProposedCluster.size();i++) {
			x1 = ProposedCluster[i][0].xX;
			y1 = ProposedCluster[i][0].yY;
			for (int j = 1; j < ProposedCluster[i].size();j++) {
				FullFilment += ProposedCluster[i][j].Stocks;
				if (MaxDistance < DetermineDistance(x1, ProposedCluster[i][j].xX, y1, ProposedCluster[i][j].yY))
					MaxDistance = DetermineDistance(x1, ProposedCluster[i][j].xX, y1, ProposedCluster[i][j].yY);
			}
			auxDist += MaxDistance;
			if ((ProposedCluster[i][0].Stocks - FullFilment) < 0)
				return 60000;
			else
				auxFull += (ProposedCluster[i][0].Stocks - FullFilment)/ ProposedCluster[i][0].Stocks;
			MaxDistance = 0;
			FullFilment = 0;
			Deliv += ProposedCluster[i].size();
		}

		return TempValD * auxDist /(60 * ProposedCluster.size()) + TempValF * auxFull;
	}
	
}

double Clusters::DetermineDistance(long double x1, long double x2, long double y1, long double y2)
{
	return sqrt(pow((x2-x1),2)+pow((y2-y1),2));
}

