#include "IdentifyClusters.h"
#include <stdlib.h>
#include <vector>
#include <unordered_set>


std::vector<std::unordered_set<SCell>> IdentifyClusters(const SMap * map, int player){
    // IdentifyCluster : Fonction qui renvoie les clusters des cellules d'un joueur, un cluster correspond 
    // à un ensemble de cellule qui forment un territoire (cellules côte à côte)
    // Paramètres :
    //      map (const SMap *) : la carte de jeu à un instant de la partie
    //      player (int): l'id du joueur
    // Sortie :
    //      clusters (std::vector<std::unordered_set<SCell>>) : clusters des cellules appartenant à un joueur
    std::vector<std::unordered_set<SCell>> clusters; 
    std::vector<SCell> cellsPlayer = PlayerCells(map, player);
    for (int i = 0; i< cellsPlayer.size();i++){
        //trouver le cluster qui contient cellspPlayer[i]
        for (std::unordered_set<SCell> clust : clusters){
            if (clust.count(cellsPlayer[i]) >= 1){ //la cellule est dans le cluster
                //ajout de tous les voisins de la cellules qui appartienent au joueur
                for (int j =0 ; j< cellsPlayer[i].nbNeighbors; j ++){
                    if (cellsPlayer[i].neighbors[j]->infos.owner == player && clust.count(cellsPlayer[i]) >=1){
                        clust.insert(cellsPlayer[i]);
                    }
                }
            }
            else {
                std::unordered_set<SCell> cluster{cellsPlayer[i]};
                clusters.push_back(cluster);
            }
        }
    }
    return clusters;
}

std::vector<SCell> PlayerCells(const SMap *map, int player){
    // PlayerCells : Fonction qui calcul les cellules appartenant à un joueur
    //
    // Paramètres : 
    //      map (const SMap*) : la carte à un instant de la partie
    //      player(int) : identifiant du joueur
    //
    // Sortie :
    //      cellsPlayer (std::vector<SCell>) : l'ensemble des cellules appartenant au joueur
    std::vector<SCell> cellsPlayer;
    for (int i = 0; i< map->nbCells ; i++){
        if (map->cells[i].infos.owner == player){
            cellsPlayer.push_back(map->cells[i]);
        }
    }
    return cellsPlayer;
}