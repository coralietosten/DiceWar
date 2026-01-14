#include "Utilities.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <stack>
#include <iterator>


std::vector<std::vector<SCell>> IdentifyClusters(const std::vector<SCell>& cellsPlayer, unsigned int player){
    // IdentifyCluster : Fonction qui renvoie les clusters des cellules d'un joueur, un cluster correspond 
     // à un ensemble de cellules qui forment un territoire (cellules côte à côte)
     // Paramètres :
     //      cellsPlayer (const std::vector<SCell>&) : les cellules qu'occupe le joueur
     //      player (unsigned int): l'id du joueur
     // Sortie :
     //      clusters (std::vector<std::set<SCell>>) : clusters des cellules appartenant à un joueur
    
    std::vector<std::vector<SCell>> clusters;

    std::vector<SCell> visited;

    for (int i =0; i< cellsPlayer.size(); i++){
        if (!contains(visited, cellsPlayer[i])){
            std::vector<SCell> cluster;
            
            //parcours en prodondeur
            std::stack<SCell> p;
            SCell s;
            p.push(cellsPlayer[i]);
            while (!p.empty()){
                s = p.top();
                p.pop();
                if (!contains(visited, s)){
                    visited.push_back(s);
                    cluster.push_back(s);
                    for (int j =0; j< s.nbNeighbors; j++){
                        SCell* neighbor = s.neighbors[j];
                        if ( neighbor->infos.owner == player && !contains(visited, *neighbor)){
                            p.push(*neighbor);
                        }
                    }
                }
            }
            clusters.push_back(cluster);
        }
    }
    return clusters;
}


std::vector<SCell> PlayerCells(const SMap& map, unsigned int player){
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

bool contains(const std::vector<SCell>& v, const SCell& c2){
    return std::find_if(v.begin(), v.end(),[&](const SCell& c){ return is_egal(c, c2);}) != v.end();
}

Connection* SCellToConnect (const SMap& map, unsigned int player){
    // critères de connection de 2 cellules :
    // territoires les plus proches
    // avec le plus de dés
    // rejoindre les clusters qui regroupe le plus de cellules
    // avec le moins de territoires adjacents adverses/ avec le moins de territoires adjacents forts(avec plus de dé que nos territoires)
    std::vector<std::vector<SCell>> clusters = IdentifyClusters(PlayerCells(map, player), player);

    //tri de clusters par taille des clusters
    std::sort(clusters.begin(), clusters.end(), [](std::vector<SCell> v1, std::vector<SCell> v2){
        return v1.size() > v2.size();
    });

    //cas ou il n'ya pas qu'un seul cluster 
    if (clusters.size() > 1){
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
                        c.distance = distance(map, clusters[i][k], clusters[j][l]);

                        if (c.distance != 0){
                            connections.push_back(c);
                        }
                    }
                }
            }
        }
        

        //tri de connections par distance
        std::sort(connections.begin(), connections.end(), [](Connection c1, Connection c2){
            return c1.distance < c2.distance;
        });
        
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
            return &connections[i-1];
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