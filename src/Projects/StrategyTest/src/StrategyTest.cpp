// #pragma once

#include "StrategyTest.h"
#include <iostream>
#include "Utilities.h"
#include <stdlib.h>
#include <vector>
#include <set>



StrategyTest::StrategyTest(unsigned int id, unsigned int nbPlayer, const SMap* map) 
    : Id(id), NbPlayer(nbPlayer)
{
    Map.nbCells = map->nbCells;
    Map.cells = new SCell[Map.nbCells];

    for (unsigned int i = 0; i < Map.nbCells; i++) {
        Map.cells[i].infos = map->cells[i].infos;
        Map.cells[i].nbNeighbors = map->cells[i].nbNeighbors;

        if (Map.cells[i].nbNeighbors > 0) {
            Map.cells[i].neighbors = new pSCell[Map.cells[i].nbNeighbors];
        } else {
            Map.cells[i].neighbors = nullptr;
        }
    }

    for (unsigned int i = 0; i < Map.nbCells; i++) {
        for (unsigned int j = 0; j < Map.cells[i].nbNeighbors; j++) {
            SCell* originalNeighbor = map->cells[i].neighbors[j];
            unsigned int index = 0;
            bool found = false;
            for (; index < map->nbCells; index++) {
                if (&map->cells[index] == originalNeighbor) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                Map.cells[i].neighbors[j] = nullptr;
            } else {
                Map.cells[i].neighbors[j] = &Map.cells[index];
            }
        }
    }
}

StrategyTest::~StrategyTest()
{
	delete[] Map.cells ;
	Map.nbCells = 0 ;
}

void StrategyTest::update(const SGameState* state) {
    for (int i = 0; i < state->nbCells; i++) {
        const SCellInfo& cell = state->cells[i];

        for (int j = 0; j < Map.nbCells; j++) {
            SCell& cellMap = Map.cells[j];

            if (cell.id == cellMap.infos.id) {            
                cellMap.infos.owner = cell.owner;
                cellMap.infos.nbDices = cell.nbDices;
                break;
            }
        }
    }
}

bool StrategyTest::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	update(state);
	// d�rouler votre algorithme de choix
	if (gameTurn == 1) {
		std::vector<SCell> territoire_joueur_1 = PlayerCells(Map, 0);
		// std::cout << " est la liste des territoires du joueur 1" << territoire_joueur_1;
		std::vector<SCell> territoire_joueur_2 = PlayerCells(Map, 1);
		// std::cout << "ceci est la liste des territoires du joueur 2" << territoire_joueur_2;
		std::vector<std::vector<SCell>> clusters = IdentifyClusters(territoire_joueur_2, 1);
		std::cout << clusters;
		std::cout <<std::endl;

		// std::cout << WidthPath(Map,territoire_joueur_1[0]);

		Connection* c = SCellToConnect(Map, 1);
		std::cout << *c << std::endl;

	
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
			// std::cout << "id cellule : " << Map.cells[i].infos.id << " ,id joueur : " << Map.cells[i].infos.owner << std::endl;
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
				// std::cout<<"Min : "<<min<<" mine: "<< nb<<std::endl;

				// if (min<nb){
				// 	turn->cellFrom = Map.cells[i].infos.id;
				// 	turn->cellTo = Map.cells[i].neighbors[neig]->infos.id;
				// 	std::cout<<"ATTACK : "<<turn->cellFrom<<" TO "<< turn->cellTo<<std::endl;

				// 	return true;
				// }
				
			}
		}
		return(false);
	}
	else {
		return (false);
	}
}
