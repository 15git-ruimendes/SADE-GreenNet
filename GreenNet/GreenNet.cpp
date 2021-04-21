#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Clustering.h"
#include "TSM.h"

#define NumArmz 100
#define NumLojas 200
#define NumParam 4

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

	int TexArm=0, TexLoj=0;

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
	p.ParseFile(ArmazensTxt, ";",ArmazensSplit,TexArm);
	p.ParseFile(LojasTxt, ";", LojasSplit,TexLoj);
	std::cout << "Files Parsed \n";


	std::vector<Local> ArmazemVect;
	std::vector<Local> LojaVect;
	for (int i = 0; i < ArmazensSplit.size()-1;i++) {
		Local aux(ArmazensSplit[i],'A');
		ArmazemVect.push_back(aux);
	}
	for (int i = 0; i < LojasSplit.size()-1; i++) {
		Local aux(LojasSplit[i], 'L');
		LojaVect.push_back(aux);
	}
	std::cout << "Local Matrixes Done\n";

	Clusters Clust;
	Clust.OptimizeClusters(ArmazemVect, LojaVect);
	Clust.printClusterCurrent();

	return 0;
}