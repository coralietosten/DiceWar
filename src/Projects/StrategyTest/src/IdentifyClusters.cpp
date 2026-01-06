#include "IdentifyClusters.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <iterator>


// ->>>>>> voir le cas ou tous les territoires appartiennent au joueur <<<<<<<<<-
// std::vector<std::vector<SCell>> IdentifyClusters(const SMap& map, int player){
//     // IdentifyCluster : Fonction qui renvoie les clusters des cellules d'un joueur, un cluster correspond 
//     // à un ensemble de cellule qui forment un territoire (cellules côte à côte)
//     // Paramètres :
//     //      map (const SMap *) : la carte de jeu à un instant de la partie
//     //      player (int): l'id du joueur
//     // Sortie :
//     //      clusters (std::vector<std::set<SCell>>) : clusters des cellules appartenant à un joueur
//     std::vector<SCell> cellsPlayer = PlayerCells(map, player);
//     std::vector<std::vector<SCell>> clusters;
//     std::vector<SCell> c ;
//     c.push_back(cellsPlayer[0]);
//     // clusters.push_back(c);
//     for (int i = 1; i < cellsPlayer.size(); i++){
//         //trouver le cluster qui contient cellspPlayer[i]
//         for (std::vector<SCell> clust : clusters){ 
//             if (contains(clust, cellsPlayer[i])){ //la cellule est dans le cluster
//                 //ajout de tous les voisins de la cellules qui appartienent au joueur
//                 for (int j = 0; j< cellsPlayer[i].nbNeighbors; j ++){
//                     if (cellsPlayer[i].neighbors[j]->infos.owner == player && !contains(clust, cellsPlayer[i])){
//                         clust.push_back(cellsPlayer[i]);
//                     }
//                 }
//             }
//             else {
//                 std::vector<SCell> cluster{cellsPlayer[i]};
//                 clusters.push_back(cluster);
//             }
//         }
//     }
//     return clusters;
// }


std::vector<std::vector<SCell>> IdentifyClusters(const std::vector<SCell>& cellsPlayer, unsigned int player){
    // IdentifyCluster : Fonction qui renvoie les clusters des cellules d'un joueur, un cluster correspond 
    // à un ensemble de cellule qui forment un territoire (cellules côte à côte)
    // Paramètres :
    //      cellsPlayer (const std::vector<SCell>&) : les cellules qu'occupe le joueur
    //      player (unsigned int): l'id du joueur
    // Sortie :
    //      clusters (std::vector<std::set<SCell>>) : clusters des cellules appartenant à un joueur
    std::vector<std::vector<SCell>> clusters; 
    if (cellsPlayer.size() == 0){
        return clusters;
    }
    //recuperation des voisins de chaque cellule
    for (int i = 0; i < cellsPlayer.size(); i++){
        std::cout << cellsPlayer[i] << std::endl;
        std::vector<SCell> c;
        c.push_back(cellsPlayer[i]);
        for (int k = 0; k < cellsPlayer[i].nbNeighbors; k ++){
            SCell *voisin = cellsPlayer[i].neighbors[k];
            std::cout << "voisin : " << *voisin;
            if(voisin != nullptr){
                unsigned int n = voisin->infos.owner;
                if (n == player){
                    c.push_back(*voisin);
                }
            }
        }
        std::cout << c << std::endl;
        //ajout du vecteur des voisins dans clusters
        if (clusters.empty()){
            clusters.push_back(c);
        }
        else{
            bool in_cluster = false;
            int j = 0;
            while (in_cluster == false && j < clusters.size() ){
                for (int l = 0; l < c.size(); l++){
                    if (contains(clusters[j], c[l])){
                        in_cluster = true;
                        std::vector<SCell> temp;
                        std::sort(clusters[j].begin(), clusters[j].end(), comparator);
                        std::sort(c.begin(), c.end(), comparator);
                        std::set_union(clusters[j].begin(), clusters[j].end(), c.begin(), c.end(), std::back_inserter(temp),comparator);
                        clusters[j] = temp;
                        break;
                    }
                }            
                j++;
            }
            if (in_cluster ==false ){
                clusters.push_back(c);
            }
        }
    }
    return clusters;
}


std::vector<SCell> PlayerCells(const SMap& map, int player){
    // PlayerCells : Fonction qui calcul les cellules appartenant à un joueur
    //
    // Paramètres : 
    //      map (const SMap*) : la carte à un instant de la partie
    //      player(int) : identifiant du joueur
    //
    // Sortie :
    //      cellsPlayer (std::vector<SCell>) : l'ensemble des cellules appartenant au joueur
    std::vector<SCell> cellsPlayer;
    for (int i = 0; i< map.nbCells ; i++){
        if (map.cells[i].infos.owner == player){
            cellsPlayer.push_back(map.cells[i]);
        }
    }
    return cellsPlayer;
}

std::ostream& operator<<(std::ostream& os, const std::vector<SCell>& v){
    os << '[';
    for (int i = 0; i < v.size(); i++){
        os << '(' << v[i].infos.id << ',' << v[i].infos.owner << ')' << ',';
    }
    os << ']' << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<SCell>>& v){
    os << '[';
    for (std::vector<SCell> vec : v){
        os << vec << ',';
    }
    os << ']' << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const SCell& c){
    os << '(' << c.infos.id << ',' << c.infos.owner << ')' << std::endl;
    return os;
}

bool is_egal(const SCell& c1, const SCell& c2){
    return c1.infos.id == c2.infos.id;  
}

bool comparator(const SCell& c1, const SCell& c2){
    return c1.infos.id < c2.infos.id;
}

bool contains(const std::vector<SCell>& v, const SCell& c2){
    return std::find_if(v.begin(), v.end(),[&](const SCell& c){ return is_egal(c, c2);}) != v.end();
}
