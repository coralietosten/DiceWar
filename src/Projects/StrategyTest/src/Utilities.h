#pragma once 

#include "../../../Commun/IStrategyLib.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include "AStar.h"
#include "Proba.h"

#include <algorithm>
#include <queue>
#include <stack>
#include <iterator>

#include <unordered_set>
#include <map>
#include <optional>

// Fonction pour réunir deux territoires en utilisant l'algorithme A*
// si on recoit false alors il faut supprimer le chemin
bool ReunirTerritoire(ResultatChemin& resultat, const SMap& map, STurn& turn);

// Fonction pour décider s'il faut attaquer ou non en fonction des probabilités et des états des cellules
// si false on passe notre tour
bool AttackOrNot(const SMap& map, std::vector<std::vector<SCell>>& nosCellules);





struct Connection{
    SCell Depart;
    SCell Arrivee; 
    int distance;
};

std::vector<std::vector<SCell>> IdentifyClusters(const std::vector<SCell>& cellsPlayer, unsigned int player);

std::vector<SCell> PlayerCells(const SMap& map, unsigned int player);

std::ostream& operator<<(std::ostream& os, const SCell& c);

std::ostream& operator<<(std::ostream& os, const std::vector<SCell>& v);

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<SCell>>& v);

std::ostream& operator<<(std::ostream& os, const Connection& c);

std::ostream& operator<<(std::ostream& os, const std::vector<Connection>& c);

bool is_egal(const SCell& c1,const SCell& c2);

bool contains(const std::vector<SCell>& v, const SCell& c2);


std::optional<Connection> SCellToConnect (const SMap& map, unsigned int player);

int distance(const SMap& map, const SCell& c, const SCell& c2);

//parcours en largeur
std::vector<std::vector<SCell>> WidthPath(const SMap& map, const SCell& c);