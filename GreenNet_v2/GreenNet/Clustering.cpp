#include "Clustering.h"

#define TempValD 10
#define TempValF 0.5


void Clusters::OptimizeClusters(std::vector<Local> Armazens, std::vector<Local> Lojas, float sigma)
{
	std::ofstream ResultFile("Results.txt");
	clock_t init;
	double diff = 0, rho, Temp, PrevVal = 0;
	unsigned int Iter = 0, FullIt = 0;
	srand((unsigned)std::time(NULL));

	init = clock();
	CreatFirstSolution(Armazens, Lojas);
	double BestVal = EvaluateSolution('C');
	std::cout << "Valor de Avaliacao:   ";
	std::cout << BestVal << std::endl;
	printClusterCurrent('C');
	printf("=================================== %d\n", ClusterCurrent.size());
	ClusterBest = ClusterCurrent;
	while ((clock() - init) / CLOCKS_PER_SEC < 300 && Iter < 4000000) {

		CreatNeighborhood();

		if((Temp = 100 - sigma * FullIt) < 0)
			Temp = 0;

		if (EvaluateSolution('P') < BestVal) {
			BestVal = EvaluateSolution('P');
			ClusterBest = ProposedCluster;
			Iter = 0;
			std::cout << "Distnacia Total:   ";
			std::cout << BestVal << std::endl;
			printClusterCurrent('B');
			printf("=================================== %d\n", ClusterBest.size());

		}
		Iter++;
		diff = EvaluateSolution('P') - EvaluateSolution('C');
		rho = exp(-diff / Temp);
		if (rand() % 100 < rho*100 && EvaluateSolution('P') < 4000)
		{
			ClusterCurrent = ProposedCluster;
			ProposedCluster.clear();
		}
		else
			ProposedCluster.clear();

		ResultFile << EvaluateSolution('C') << ";" << Temp << ";" << rho*100 << ";" << std::endl;
		FullIt++;
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
	int A1 = 0, A2 = 0, LOg1 = 0, LOg2 = 0, Pos1 = 0, Pos2 = 0;
	std::vector<Local>::iterator I1, I2;
	Local L1, L2;

	if (rand() % 100 + 1 > 50)
	{
		//Change 2 Stores
		do
		{
			A1 = rand() % (ClusterCurrent.size());
			A2 = rand() % (ClusterCurrent.size());

			if ((ClusterCurrent[A1].size() - 1 > 0) && (ClusterCurrent[A2].size() - 1 > 0))
			{
				Pos1 = rand() % (ClusterCurrent[A1].size() - 1) + 1;
				Pos2 = rand() % (ClusterCurrent[A2].size() - 1) + 1;
				LOg1 = rand() % (ClusterCurrent[A1].size() - 1) + 1;
				LOg2 = rand() % (ClusterCurrent[A2].size() - 1) + 1;
			}
			else {
				LOg1 = 0;
				LOg2 = 0;
			}
		} while (LOg1 == 0 || LOg2 == 0 || A1 == A2);
		ProposedCluster = ClusterCurrent;
		
		I1 = ProposedCluster[A1].begin() + Pos1;
		I2 = ProposedCluster[A2].begin() + Pos2;

		ProposedCluster[A2].insert(I2, ProposedCluster[A1][LOg1]);
		ProposedCluster[A1].insert(I1, ProposedCluster[A2][LOg2]);
		if (Pos1 <= LOg1)
			ProposedCluster[A1].erase(ProposedCluster[A1].begin() + LOg1 + 1);
		else
			ProposedCluster[A1].erase(ProposedCluster[A1].begin() + LOg1);
		if (Pos2 <= LOg2)
			ProposedCluster[A2].erase(ProposedCluster[A2].begin() + LOg2 + 1);
		else
			ProposedCluster[A2].erase(ProposedCluster[A2].begin() + LOg2);

	}
	else
	{
		//Change 1 Store
		do
		{
			A1 = rand() % (ClusterCurrent.size());
			A2 = rand() % (ClusterCurrent.size());

			if ((ClusterCurrent[A1].size() - 1 > 0))
			{
				LOg1 = rand() % (ClusterCurrent[A1].size() - 1) + 1;
			}
			else {
				LOg1 = 0;
			}
			if (ClusterCurrent[A2].size() > 1) {
				Pos2 = rand() % (ClusterCurrent[A2].size() - 1) + 1;
			}
			else
				Pos2 = 0;
		} while (LOg1 == 0);
		ProposedCluster = ClusterCurrent;

		if (Pos2 == 0) {
			ProposedCluster[A2].push_back(ProposedCluster[A1][LOg1]);
		}
		else {
			I2 = ProposedCluster[A2].begin() + Pos2;
			ProposedCluster[A2].insert(I2, ProposedCluster[A1][LOg1]);
		}

		ProposedCluster[A1].erase(ProposedCluster[A1].begin() + LOg1);
			
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
	double TotDist = 0, Entrgue = 0;
	if (Qual == 'C') 
	{
		for (int i = 0; i < ClusterCurrent.size(); i++)
		{
			TotDist += CalculateFitness(ClusterCurrent[i]);
		}

		for (int i = 0; i < ClusterCurrent.size(); i++) {
			for (int j = 1; j < ClusterCurrent[i].size(); j++) {
				for (int k = 0; k < ClusterCurrent.size(); k++) {
					for (int t = 1; t < ClusterCurrent[k].size(); t++) {
						if (ClusterCurrent[i][j].Id == ClusterCurrent[k][t].Id && (k != i || t != j))
							return 100000;
					}
				}
			}
		}
		return TotDist;
	}
	if (Qual == 'P')
	{
		for (int i = 0; i < ProposedCluster.size(); i++)
		{
			TotDist += CalculateFitness(ProposedCluster[i]);
		}

		for (int i = 0; i < ProposedCluster.size(); i++) {
			for (int j = 1; j < ProposedCluster[i].size(); j++) {
				for (int k = 0; k < ProposedCluster.size(); k++) {
					for (int t = 1; t < ProposedCluster[k].size(); t++) {
						if (ProposedCluster[i][j].Id == ProposedCluster[k][t].Id && (k != i || t != j))
							return 100000;
					}
				}
			}
		}
		return TotDist;
	}
	if (Qual == 'B')
	{
		for (int i = 0; i < ClusterBest.size(); i++)
		{
			TotDist += CalculateFitness(ClusterBest[i]);
		}
		return TotDist;
	}
}

double Clusters::DetermineDistance(long double x1, long double x2, long double y1, long double y2)
{
	return sqrt(pow((x2-x1),2)+pow((y2-y1),2));
}
double Clusters::CalculateFitness(std::vector<Local> Prop)
{
	double aux = 0,StockCheck = 0;
	int id[86] = { 0 };

	for (int i = 0; i < Prop.size() - 1; i++)
	{
		aux += DetermineDistance(Prop[i].xX, Prop[i + 1].xX, Prop[i].yY, Prop[i + 1].yY);
	}
	aux += DetermineDistance(Prop[Prop.size() - 1].xX, Prop[0].xX, Prop[Prop.size() - 1].yY, Prop[0].yY);
	for (int i = 1; i < Prop.size(); i++) 
	{
		id[i] = Prop[i].Id;
		StockCheck += Prop[i].Stocks;
		if (StockCheck > Prop[0].Stocks)
			return 100000000;		
	}	
	return aux;
}

