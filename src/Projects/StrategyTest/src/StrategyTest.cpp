#include "StrategyTest.h"
#include <iostream>

StrategyTest::StrategyTest(unsigned int id, unsigned int nbPlayer, const SMap* map) :
	Id(id),
	NbPlayer(nbPlayer)
{
	// faire une copie enti�re de la structure map localement dans l'objet Map
	Map.nbCells = map->nbCells;
	Map.cells = new SCell[map->nbCells];
	for(int i=0;i<Map.nbCells;i++){
		Map.cells[i] = map->cells[i];
	}
}

StrategyTest::~StrategyTest()
{
	delete[] Map.cells ;
	Map.nbCells = 0 ;
}

bool StrategyTest::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	// d�rouler votre algorithme de choix
	std::cout<<"NB JOUEURS : "<<NbPlayer<<std::endl;
	std::cout<<"Id : "<<Id<<std::endl;

	std::cout<<"Dés : ";
	for(int i=0;i<NbPlayer;i++){
		std::cout<<state->diceStock[i];
		if (i<NbPlayer-1){
			std::cout<<",";
		}
	}
	std::cout<<std::endl;

	std::cout<<"Points : ";
	for(int i=0;i<NbPlayer;i++){
		std::cout<<state->points[i];
		if (i<NbPlayer-1){
			std::cout<<",";
			
		}
	}
	std::cout<<std::endl;
	std::cout<<"Turn : "<<gameTurn<<std::endl;



	for(int i = 0; i<Map.nbCells;i++){
		if(Map.cells[i].infos.owner == Id){
			int nb = Map.cells[i].infos.nbDices;
			

			int min = -1;
			int neig = -1;
			for(int j=0;j<Map.cells[i].nbNeighbors;j++){
				if (Map.cells[i].neighbors[j]->infos.owner != Map.cells[i].infos.owner ){
					int nbDices = Map.cells[i].neighbors[j]->infos.nbDices;
					if (min == -1){
						min = nbDices;
						neig = j;
					}else if(min>nbDices){
						min = nbDices;
						neig = j;
					}
				}
			}
			std::cout<<"Min : "<<min<<" mine: "<< nb<<std::endl;

			if (min<nb){
				turn->cellFrom = Map.cells[i].infos.id;
				turn->cellTo = Map.cells[i].neighbors[neig]->infos.id;
				std::cout<<"ATTACK : "<<turn->cellFrom<<" TO "<< turn->cellTo<<std::endl;

				return true;
			}
			
		}
	}
	return(false);
}
