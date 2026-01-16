#pragma once
#include <vector>
#include "../../../Commun/IStrategyLib.h"
#include "Proba.h"
#include <vector>
#include <iostream>
#include <algorithm> // Pour std::find

// Définition de la structure pour le résultat du chemin -> si résultat trop bas on peut ne pas les faire se rejoindre
struct ResultatChemin {
    std::vector<SCell> chemin;
    double probabilite;
};

void _AStar(SCell* current, const SCell* goal, std::vector<SCell>& path, double current_prob, int current_dice, std::vector<int>& visited_ids,double& g_max_prob,std::vector<SCell>& g_best_path );

ResultatChemin AStar(const SMap map, const SCell& start, const SCell& goal);
