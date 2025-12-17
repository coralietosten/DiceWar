#pragma once 

#include "../../../Commun/IStrategyLib.h"

std::vector<std::unordered_set<SCell>> IdentifyClusters(const SMap * map, int player); 

std::vector<SCell> PlayerCells(const SMap *map, int player);