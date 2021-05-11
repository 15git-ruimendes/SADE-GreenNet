#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Clustering.h"
#include "TSM.h"

#define NumArmz 100
#define NumLojas 200
#define NumParam 4

double Distance(long double x1, long double x2, long double y1, long double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

double CheckPathDistance(std::vector<Local> Path) {
	double Dist = 0;
	for (int i = 0; i < Path.size() - 1; i++) {
		Dist += Distance(Path[i].xX, Path[i + 1].xX, Path[i].yY, Path[i + 1].yY);
	}
	Dist += Distance(Path[Path.size() - 1].xX, Path[0].xX, Path[Path.size() - 1].yY, Path[0].yY);
	return Dist;
}

double CheckStock(std::vector<Local> Path) {
	double Entregue = 0;
	for (int i = 1; i < Path.size(); i++)
	{
		Entregue += Path[i].Stocks;
	}
	return Entregue;
}

void FinalStockCheck(std::vector<std::vector<Local>> BestClust,int *checkstock,float *maxdist1,int *lojasAbas,float *avgFullFill) 
{
	
	(*checkstock) = 1;
	long double stocks, maxdist=0, maxdistaux=0,fullfillaux=0;
	for (int i = 0; i < BestClust.size() ; i++)
	{
		stocks = BestClust[i][0].Stocks;
		maxdist += CheckPathDistance(BestClust[i]);
		for (int j = 1; j < BestClust[i].size(); j++) 
		{
			stocks -= BestClust[i][j].Stocks;
			if (stocks < 0)
				(*checkstock) = 0;
		}

		fullfillaux += 1 - (stocks) / BestClust[i][0].Stocks;
		(*lojasAbas) += (BestClust[i].size()-1);
	}
	(*maxdist1) = (float) maxdist;
	(*avgFullFill) = (float)(fullfillaux /( BestClust.size()));
}



class Parse
{
public:
	void ParseFile(std::vector<std::string> Array, std::string Delim,std::vector<std::vector<long double>>& Res,int Size) {
			
		for (int i = 0; i < Size;i++) {
			Res.resize(i + 1);
			size_t pos = 0;
			std::string token;
			int j = 0;
			while ((pos = Array[i].find(Delim)) != std::string::npos) {
				token = Array[i].substr(0, pos);
				
				if (i >= 1) {
					(Res)[i - 1].push_back(std::stold(token));
				}
				j++;
				Array[i].erase(0, pos + Delim.length());
			}
			if (i >= 1) {
				(Res)[i - 1].push_back(std::stold(Array[i]));
			}
			
		}
	}
};




int main() {
	std::cout << "Running ! \n";

	Parse p;

	int TexArm = 0, TexLoj = 0;

	std::vector<std::string> ArmazensTxt(NumArmz);
	std::fstream ArmzFile("Dados/Armazens.txt");
	for (int i = 0; std::getline(ArmzFile, ArmazensTxt[i]);i++)
	{
		TexArm++;
		//std::cout << ArmazensTxt[i];
		//std::cout << "\n";
	}
	std::vector<std::string> LojasTxt(NumLojas);
	std::fstream LojFile("Dados/Lojas.txt");
	for (int i = 0; std::getline(LojFile, LojasTxt[i]);i++)
	{
		TexLoj++;
		//std::cout << LojasTxt[i];
		//std::cout << "\n";
	}
	std::cout << "Files Read \n";

	std::vector<std::vector<long double>> ArmazensSplit;
	std::vector<std::vector<long double>> LojasSplit;
	p.ParseFile(ArmazensTxt, ";", ArmazensSplit, TexArm);
	p.ParseFile(LojasTxt, ";", LojasSplit, TexLoj);
	std::cout << "Files Parsed \n";


	std::vector<Local> ArmazemVect;
	std::vector<Local> LojaVect;
	for (int i = 0; i < ArmazensSplit.size() - 1;i++) {
		Local aux(ArmazensSplit[i], 'A');
		ArmazemVect.push_back(aux);
	}
	for (int i = 0; i < LojasSplit.size() - 1; i++) {
		Local aux(LojasSplit[i], 'L');
		LojaVect.push_back(aux);
	}
	std::cout << "Local Matrixes Done\n";

	Clusters Clust;
	printf("Lojas Iniciais %d\n", LojaVect.size());
	Clust.OptimizeClusters(ArmazemVect, LojaVect, 0.1);
	//Clust.printClusterCurrent('B');

	int checkstock = 0,lojasAbst=0;
	float maxdist = 0,avgFulFill =0;
	FinalStockCheck(Clust.ClusterBest,&checkstock,&maxdist,&lojasAbst,&avgFulFill);
	printf("Are clusters compatible with stocks: %d  ; Total Distance:  %f; Fullfiled Shops:  %d; Avg Usage :  %f %%\n", checkstock, maxdist,lojasAbst,(avgFulFill*100));

	std::cout << "===================================\n";
	std::cout << "Final Paths Being Printed! \n";
	for (int i = 0; i < Clust.ClusterBest.size(); i++) {
		for (int j = 0; j < 10; j++) {
			if (j < Clust.ClusterBest[i].size())
				std::cout << Clust.ClusterBest[i][j].Id << "\t";
			else
				std::cout << "\t";
		}
		std::cout << CheckPathDistance(Clust.ClusterBest[i]) << "   Capacity :" << Clust.ClusterBest[i][0].Stocks << "   Delivered:  " << CheckStock(Clust.ClusterBest[i])<<std::endl;
	}

	return 0;
}