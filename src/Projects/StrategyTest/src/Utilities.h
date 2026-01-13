#pragma once 

#include "../../../Commun/IStrategyLib.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>


struct Connection{
    SCell Depart;
    SCell Arrivee; 
    int distance;
};

// std::vector<std::vector<SCell>> IdentifyClusters(const std::vector<SCell>& cellsPlayer, unsigned int player);

int find(std::vector<int> parent, int x);

void union1(std::vector<int> parent, int a, int b);

std::vector<std::vector<SCell>> IdentifyClusters(const std::vector<SCell>& cellsPlayer, unsigned int player);

std::vector<SCell> PlayerCells(const SMap& map, int player);

std::ostream& operator<<(std::ostream& os, const SCell& c);

std::ostream& operator<<(std::ostream& os, const std::vector<SCell>& v);

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<SCell>>& v);

std::ostream& operator<<(std::ostream& os, const Connection& c);

std::ostream& operator<<(std::ostream& os, const std::vector<Connection>& c);



bool is_egal(const SCell& c1,const SCell& c2);

bool comparator(const SCell& c1, const SCell& c2);

bool comparator_connection(const Connection& c1, const Connection& c2);

bool contains(const std::vector<SCell>& v, const SCell& c2);

Connection* SCellToConnect (const SMap& map, unsigned int player);

int distance(const SMap& map, const SCell& c, const SCell& c2);

//parcours en largeur
std::vector<std::vector<SCell>> WidthPath(const SMap& map, const SCell& c);