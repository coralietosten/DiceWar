#pragma once 

#include "../../../Commun/IStrategyLib.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>

std::vector<std::vector<SCell>> IdentifyClusters(const SMap& map, int player); 

std::vector<std::vector<SCell>> IdentifyClusters(const std::vector<SCell>& cellsPlayer, unsigned int player);

std::vector<SCell> PlayerCells(const SMap& map, int player);

std::ostream& operator<<(std::ostream& os, const SCell& c);

std::ostream& operator<<(std::ostream& os, const std::vector<SCell>& v);

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<SCell>>& v);


bool is_egal(const SCell& c1,const SCell& c2);

bool comparator(const SCell& c1, const SCell& c2);

bool contains(const std::vector<SCell>& v, const SCell& c2);
