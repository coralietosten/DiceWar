#include "StrategyTest.h"

    
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
    // Libérer chaque tableau de voisins
    for (unsigned int i = 0; i < Map.nbCells; i++) {
        delete[] Map.cells[i].neighbors;
    }
    // Libérer le tableau de cellules
    delete[] Map.cells;
    Map.cells = nullptr;
    Map.nbCells = 0;
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
	

    std::vector<std::vector<SCell>> lsCluster =
    IdentifyClusters(PlayerCells(Map, Id), Id);

    if (lsCluster.size() > 1) {
        std::optional<Connection> conn = SCellToConnect(Map, Id);
        if (conn) {
            ResultatChemin res =
                AStar(Map, conn->Depart, conn->Arrivee);

            bool hasAttack = ReunirTerritoire(
                res,
                Map,
                *turn
            );

            if(hasAttack){
                return true;
            }
        }
    }

    if (!AttackOrNot(Map,lsCluster)){
        return false;
    }

	

    
	for (int i = 0; i < Map.nbCells; i++) {
		if (Map.cells[i].infos.owner != Id)
			continue;

		int myDices = Map.cells[i].infos.nbDices;

		int minEnemyDices = -1;
		int targetIndex = -1;
		// std::cout << "Cell " << i
        //   << " neighbors ptr = " << Map.cells[i].neighbors
        //   << " nbNeighbors = " << Map.cells[i].nbNeighbors
        //   << std::endl;

		for (int j = 0; j < Map.cells[i].nbNeighbors; j++) {
			pSCell neighbor = Map.cells[i].neighbors[j];

			if (!neighbor){
 				continue;
			}

			if (neighbor->infos.owner == Id){
				continue;

			}

			int enemyDices = neighbor->infos.nbDices;

			if (minEnemyDices == -1 || enemyDices < minEnemyDices) {
				minEnemyDices = enemyDices;
				targetIndex = j;
			}
		}

		if (targetIndex == -1)
        {
            continue;
        }

		if (myDices > minEnemyDices) {
			turn->cellFrom = Map.cells[i].infos.id;
			turn->cellTo   = Map.cells[i].neighbors[targetIndex]->infos.id;
			return true;
		}
	}

	return(false);
}
