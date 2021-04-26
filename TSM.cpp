#include <iostream>
#include <vector>
#include <string>
#include "TSM.h"

void Paths::OptimizePath(std::vector<std::vector<Local>> Clusts)
{
	clock_t init;
	double aux,bestaux = 10000;
	PathsDefined = Clusts;
	for (int i = 0; i < Clusts.size(); i++)
	{
		if (Clusts[i].size() > 1) {
			GenerateFirstPop(Clusts[i]);

			init = clock();
			while ((clock() - init) / CLOCKS_PER_SEC < (int)(150 / Clusts.size()))
			{
				aux = CalculateFitnessAndGenerateNew();
				if (bestaux > aux)
				{
					bestaux = aux;
					PathsDefined[i] = InitPop[0];
				}
				Generate();
			}
		}
		

		//Time Stuff
		
	}
	//Generate First Population 
	//Selection Function
	//Paring
	//Mutation
	//Evaluation


}

double Paths::CalculateFitnessAndGenerateNew()
{
	InitPop.clear();
	double res = 0, best = 10000,bestbest = 0;
	int init = Pop.size(), i = 0,aux,rand1;
	while (init - Pop.size() < 1)
	{
		if(i <= Pop.size() / 3 - 2)
		{
			for (int j = 0; j < Pop.size();j++)
			{
				
				res = CalculateFitness(Pop[j]);
				
				if (res < best) {
					aux = j;
					best = res;
					if (i == 0)
						bestbest = best;
				}
				else
					aux = rand() % Pop.size();
			}
			InitPop.push_back(Pop[aux]);
			Pop.erase(Pop.begin() + aux);
		}
		else
		{
			srand((unsigned)time(0));
			rand1 = rand() % Pop.size();
			InitPop.push_back(Pop[rand1]);
			Pop.erase(Pop.begin() + rand1);
		}
		i++;
	}
	return bestbest;
}

double Paths::CalculateFitness(std::vector<Local> Prop)
{
	double aux = 0;
	int id[86] = {0};
	for (int i = 0; i < Prop.size()-1; i++)
	{
		if (i > 0) {
			id[i-1] = (int)Prop[i].Id;
			for (int j = 0; j < i-1;j++) {
				for (int k = 0; k < i-1; k++) {
					if (id[k] == id[j] && k != j )
						return 1000000;
				}
			}	
		}
		aux += DetermineDistance(Prop[i].xX, Prop[i + 1].xX, Prop[i].yY, Prop[i + 1].yY);
	}
	//i++;
	return aux + DetermineDistance(Prop[Prop.size()-1].xX, Prop[0].xX, Prop[Prop.size()-1].yY, Prop[0].yY);
}

void Paths::Mutate()
{
}

void Paths::Generate()
{
	Pop.clear();
	Pop = InitPop;

	if (InitPop.size() % 2 && InitPop.size() > 0)
	{
		for (int i = 0; i < InitPop.size()-1;i += 2) {
			DevideNSwitch(InitPop[i], InitPop[i + 1]);
			Pop.push_back(InitPop[i]);
			Pop.push_back(InitPop[i + 1]);
		}
	}
	else if (InitPop.size() % 2 == 0 && InitPop.size() > 0)
	{
		for (int i = 0; i < InitPop.size() - 1;i += 2) {
			DevideNSwitch(InitPop[i], InitPop[i + 1]);
			Pop.push_back(InitPop[i]);
			Pop.push_back(InitPop[i + 1]);
		}
	}

}

void Paths::DevideNSwitch(std::vector<Local> Mother, std::vector<Local> Father)
{
	int cut1, cut2;
	std::vector<Local> auxM, auxF;
	if(Mother.size() == Father.size() && Father.size() % 2)
	{
		int i;
		cut1 = (Mother.size() - 1) / 2;
		for (i = 1; i < cut1; i++)
		{
			auxM.push_back(Mother[i]);
			auxF.push_back(Father[i]);
		}
		for (i = 1; i < cut1; i++)
		{
			Father[i] = auxM[i-1];
			Mother[i] = auxF[i-1];
		}

	}
	else if (Mother.size() == Father.size() && Father.size() % 2 == 0)
	{
		int i;
		cut1 = (Mother.size() - 1) / 2;
		for (i = 1; i < cut1; i++)
		{
			auxM.push_back(Mother[i]);
			auxF.push_back(Father[i]);
		}
		for (i = 1; i < cut1; i++)
		{
			Father[i] = auxM[i-1];
			Mother[i] = auxF[i-1];
		}
	}
}

void Paths::GenerateFirstPop(std::vector<Local> Clust)
{
	int i1, i2,j;
	std::vector<Local> Clust_aux;
	Clust_aux = Clust;
	for (int m = 0; m < 20; m++) {
		if (Clust_aux.size() > 1)
		{
			srand((unsigned)time(0));
			j = rand() % (Clust_aux.size() - 1);
			for (int k = 0; k < j; k++) {
				do
				{
					srand((unsigned)time(0));
					i1 = rand() % (Clust_aux.size()-1)+1;
					i2 = rand() % (Clust_aux.size()-1)+1;
				} while (i1 >= i2);
				std::iter_swap(Clust_aux.begin() + i1, Clust_aux.begin() + i2);
			}
		}
		Pop.push_back(Clust_aux);
	}
	
}

double Paths::DetermineDistance(long double x1, long double x2, long double y1, long double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

