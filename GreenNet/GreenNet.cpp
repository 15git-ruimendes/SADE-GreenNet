#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Clustering.h"
#include "TSM.h"

#define NumArmz 150
#define NumLojas 200
#define NumParam 4

class Parse
{
public:
	void ParseFile(std::vector<std::string> Array, std::string Delim,std::vector<std::vector<long double>>& Res,int Size) {
			
		for (int i = 0; i < Size;i++) {
			size_t pos = 0;
			std::string token;
			int j = 0;
			while ((pos = Array[i].find(Delim)) != std::string::npos) {
				token = Array[i].substr(0, pos);
				
				if (i >= 1) {
					(Res)[i - 1][j] = std::stold(token);
				}
				j++;
				Array[i].erase(0, pos + Delim.length());
			}
			if (i >= 1) {
				(Res)[i - 1][j] = std::stold(Array[i]);
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
	
	std::vector<std::vector<long double>> Armazens(NumArmz, std::vector<long double>(NumParam));
	std::vector<std::vector<long double>> Lojas(NumLojas, std::vector<long double>(NumParam));
	p.ParseFile(ArmazensTxt, ";",Armazens,TexArm);
	p.ParseFile(LojasTxt, ";", Lojas,TexLoj);
	std::cout << "Files Parsed \n";


	std::vector<Local> Locais;
	for (int i = 0; i < Armazens.size();i++) {
		Local aux(Armazens[i],'A');
		Locais.push_back(aux);
	}
	for (int i = 0; i < Lojas.size(); i++) {
		Local aux(Lojas[i], 'L');
		Locais.push_back(aux);
	}
	std::cout << "Local Matrixes Done";


	return 0;
}