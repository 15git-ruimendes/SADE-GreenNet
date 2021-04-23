#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Clustering.h"
#include "TSM.h"

#define NumArmz 100
#define NumLojas 200
#define NumParam 4

void FinalStockCheck(std::vector<std::vector<Local>> BestClust,int *checkstock,float *maxdist1,int *lojasAbas,float *avgFullFill) 
{
	
	(*checkstock) = 1;
	long double stocks, maxdist, maxdistaux=0,fullfillaux=0;
	for (int i = 0; i < BestClust.size() ; i++)
	{
		stocks = BestClust[i][0].Stocks;
		maxdist = 0;
		for (int j = 1; j < BestClust[i].size(); j++) 
		{
			stocks -= BestClust[i][j].Stocks;
			if (sqrt(pow((BestClust[i][j].xX - BestClust[i][0].xX), 2) + pow((BestClust[i][j].yY - BestClust[i][0].yY), 2)) > maxdist) 
			{
				maxdist = sqrt(pow((BestClust[i][j].xX - BestClust[i][0].xX), 2) + pow((BestClust[i][j].yY - BestClust[i][0].yY), 2));
			}
			if (stocks < 0)
				(*checkstock) = 0;
			
		}
		fullfillaux += 1 - (stocks) / BestClust[i][0].Stocks;
		maxdistaux += maxdist;
		(*lojasAbas) += (BestClust[i].size()-1);
	}
	(*maxdist1) =(float) (maxdistaux / (BestClust.size()));
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
	Clust.OptimizeClusters(ArmazemVect, LojaVect, 0.9);
	Clust.printClusterCurrent('B');

	int checkstock = 0,lojasAbst=0;
	float maxdist = 0,avgFulFill =0;
	FinalStockCheck(Clust.ClusterBest,&checkstock,&maxdist,&lojasAbst,&avgFulFill);
	printf("Are clusters compatible with stocks: %d  ; Avg. Max Dist:  %f; Fullfiled Shops:  %d; Avg Usage of Warehouse:  %f\n", checkstock, maxdist,lojasAbst,avgFulFill*100);

	Paths PathOptim;

	return 0;
}