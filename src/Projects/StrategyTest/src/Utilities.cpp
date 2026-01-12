#include "Utilities.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <iterator>


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
        std::vector<SCell> c;
        c.push_back(cellsPlayer[i]);
        for (int k = 0; k < cellsPlayer[i].nbNeighbors; k ++){
            SCell *voisin = cellsPlayer[i].neighbors[k];
            if(voisin != nullptr){
                unsigned int n = voisin->infos.owner;
                if (n == player){
                    c.push_back(*voisin);
                }
            }
        }
        //ajout du vecteur des voisins dans clusters
        if (clusters.empty()){
            clusters.push_back(c);
        }
        else{
            bool inCluster = false;
            int j = 0;
            while (inCluster == false && j < clusters.size() ){
                for (int l = 0; l < c.size(); l++){
                    if (contains(clusters[j], c[l])){
                        inCluster = true;
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
            if (inCluster ==false ){
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

std::ostream& operator<<(std::ostream& os, const Connection& c){
    os << "départ : " << c.Depart << " arrivée : " << c.Arrivee << " distance" << c.distance << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Connection>& c){
    os << '[';
    for (int i = 0; i < c.size(); i++){
        os << c[i] << ',';
    }
    os << ']' << std::endl;
    return os;
}


bool is_egal(const SCell& c1, const SCell& c2){
    return c1.infos.id == c2.infos.id;  
}

bool comparator(const SCell& c1, const SCell& c2){
    return c1.infos.id < c2.infos.id;
}

bool comparator_connection(const Connection& c1, const Connection& c2){
    return c1.distance < c2.distance;
}


bool contains(const std::vector<SCell>& v, const SCell& c2){
    return std::find_if(v.begin(), v.end(),[&](const SCell& c){ return is_egal(c, c2);}) != v.end();
}

Connection* SCellToConnect (const SMap& map, unsigned int player){
    // critères de connection de 2 cellules :
    // territoires les plus proches
    // avec le plus de dés
    // avec le moins de territoires adjacents adverses/ avec le moins de territoires adjacents forts(avec plus de dé que nos territoires)
    // rejoindre les clusters qui regroupe le plus de cellules
    std::vector<std::vector<SCell>> clusters = IdentifyClusters(PlayerCells(map, player), player);

    //cas ou il n'ya pas qu'un seul cluster 
    if (clusters.size() > 1){
        std::cout << "ici" << std::endl;
        std::vector<Connection> connections;
        for (int i = 0; i< clusters.size(); i++){
            for (int j = i+1; j < clusters.size(); j++){
                for (int k = 0; k < clusters[i].size(); k++){
                    for (int l = 0; l< clusters[j].size(); l++){
                        //ajout de toutes les paires dans connections
                        Connection c;
                        if (clusters[i][k].infos.nbDices < clusters[j][l].infos.nbDices){
                            c.Arrivee = clusters[i][k];
                            c.Depart = clusters[j][l];
                        }
                        else{
                            c.Arrivee = clusters[j][l];
                            c.Depart = clusters[i][k];
                        }
                        // std::cout << "ici " << c.Arrivee << c.Depart << std::endl;
                        c.distance = distance(map, clusters[i][k], clusters[j][l]);
                        // std::cout <<c ;

                        if (c.distance != 0){
                            connections.push_back(c);
                        }
                    }
                }
            }
        }
        

        //tri de connections par distance
        std::sort(connections.begin(), connections.end(), comparator_connection);

        //vérification s'il y a plusieurs distance min
        int i = 0;
        int nbDices = 0;
        while (i < connections.size() && connections[i].distance == connections[0].distance){
            if (connections[i].Arrivee.infos.nbDices + connections[i].Depart.infos.nbDices > nbDices){
                nbDices = connections[i].Arrivee.infos.nbDices + connections[i].Depart.infos.nbDices;
            }
            i+=1;
        }
        if (i < connections.size()){
            return &connections[i];
        }
    }
    return NULL;
}

int distance(const SMap& map, const SCell& c, const SCell& c2){
    //distance entre c et c2
    std::vector<std::vector<SCell>> parcours = WidthPath(map, c);
    for (int i = 0; i < parcours.size(); i ++){
        if (contains(parcours[i], c2)){
            return i;
        }
    }
    return 0;
}


std::vector<std::vector<SCell>> WidthPath(const SMap& map, const SCell& c){
    //parcour en largeur par "couche"
    std::vector<std::vector<SCell>> result;
    std::queue<SCell> f;
    f.push(c);
    std::vector<SCell> visited;
    visited.push_back(c);
    while(!f.empty()){
        int length = f.size();
        std::vector<SCell> temp;
        for (int i = 0; i< length; i++){
            SCell s = f.front();
            f.pop();
            for (int i = 0; i< s.nbNeighbors; i++){
                SCell *neighbor = s.neighbors[i];
                if (!contains(visited, *neighbor)){
                    f.push(*neighbor);
                    visited.push_back(*neighbor);
                    temp.push_back(*neighbor);
                }
            }
        }
        result.push_back(temp);
    }
    return result;
}