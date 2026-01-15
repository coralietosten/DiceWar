#pragma once
#include "../../../Commun/IStrategyLib.h"
#include <vector>
#include "AStar.h"
#include "Proba.h"

// Fonction pour réunir deux territoires en utilisant l'algorithme A*
// si on recoit false alors il faut supprimer le chemin
bool ReunirTerritoire(ResultatChemin& resultat, const SMap& map, STurn& turn);

// Fonction pour décider s'il faut attaquer ou non en fonction des probabilités et des états des cellules
// si false on passe notre tour
bool AttackOrNot(const SMap& map, const STurn& turn, std::vector<std::vector<SCell>>& nosCellules);


