Ce code est à mettre APRÈS le code de CodeRelierLesTerritoires.md


le .h:
```
#pragma once
#include "IStrategyLib.h"
#include <vector>


// Fonction pour décider s'il faut attaquer ou non en fonction des probabilités et des états des cellules
// si false on passe notre tour
bool AttaqueOrNot(const SMap& map, const STurn& turn, std::vector<std::vector<SCell>>& nosCellules);
```

le .cpp:
```
##include "AttaqueOrNot.h"
#include <iostream>
#include <map>
#include <vector>

bool AttaqueOrNot(const SMap& map, std::vector<std::vector<SCell>>& nosCellules)
{

    if (nosCellules.empty() || nosCellules[0].empty()) return false;
    unsigned int mon_id = nosCellules[0][0].infos.owner;

    // LOGIQUE 1 : EST ON TROP FAIBLE POUR ATTAQUER ?

    double somme_nos_des = 0.0;
    int nombre_nos_cellules = 0;

    for (const auto& region : nosCellules) {
        for (const auto& cell : region) {
            somme_nos_des += cell.infos.nbDices;
            nombre_nos_cellules++;
        }
    }
    double notre_moyenne = somme_nos_des / nombre_nos_cellules;

    // Si on est en dessous de 4 dés de moyenne, on se compare aux autres
    if (notre_moyenne < 4.0) 
    {
        double somme_total_des = 0.0;
        for (unsigned int i = 0; i < map.nbCells; ++i) {
            somme_total_des += map.cells[i].infos.nbDices;
        }

        double moyenne_globale = somme_total_des / map.nbCells;

        // Si la moyenne mondiale (+30%) est supérieure à la nôtre
        // OU si on est vraiment critique (< 3.0) -> ON N'ATTAQUE PAS
        if ((moyenne_globale * 1.3) > notre_moyenne || notre_moyenne < 3.0) {
            return false; 
        }
    }


    // LOGIQUE 2 : IL FAUT Q'ON ARRETE D4ATTENDRE

    //pour eviter de faire tout les calculs par la suite
    if(notre_moyenne >= 7.0) 
    {
        return true; // On est très fort, on attaque tout !
    }


    // LOGIQUE 3 : SOMMES NOUS ASSEZ DOMINANTS POUR ATTENDRE ?

    // Compter les territoires de tout le monde
    std::map<unsigned int, int> compte_territoires;
    compte_territoires[mon_id] = 0; // Init

    for (unsigned int i = 0; i < map.nbCells; ++i) {
        compte_territoires[map.cells[i].infos.owner]++;
    }

    // Nettoyage des joueurs n'ayant plus de territoires au cas où 
    for( int i = 0; i < (int)compte_territoires.size(); i++)
    {
        if(compte_territoires[i] == 0) 
        {
            compte_territoires.erase(i);
        }
    }

    // calcul de l'ennemi le plus dangereux
    int nombre_nos_territoires = compte_territoires[mon_id];

    int max_territoires_ennemi = 0;
    int nb_joueurs_actifs = 0; 

    for (auto const& [id_joueur, nb_territoires] : compte_territoires) 
    {
        // On compte les joueurs (qui ont au moins 2 territoires)
        if (nb_territoires >= 2) {
            nb_joueurs_actifs++;
        }

        if (id_joueur == mon_id) continue;    
        if (nb_territoires == 0) continue;

        if (nb_territoires > max_territoires_ennemi) {
            max_territoires_ennemi = nb_territoires;
        }
    }


    int nombre_terretoires_total = map.nbCells;

    //LOGIQUE 3.5 : SI ON EST PAS ASSEZ DOMINANT, ON ATTAQUE
    if(nombre_nos_territoires < (nombre_terretoires_total / nb_joueurs_actifs))
    {
        return true; // on est pas assez dominant, on attaque
    }


    double coefficient_domination = 1.3; // il faudra surment l'ajuster
    
    bool condition_domination = (nombre_nos_territoires > (max_territoires_ennemi * coefficient_domination));
    bool condition_force = (notre_moyenne >= 7.0);  // on remet au cas où
    bool condition_monde = (nb_joueurs_actifs < 3);   // si y'a que 2 joueurs = on est en duel il faut attaquer

    if (condition_domination && condition_force && condition_monde) 
    {
        return true; // Tous les feux sont au vert, on écrase tout !
    }
    else 
    {
        // Si une seule condition manque (trop faibel ou pas assez dominant), on reste défensif pour accumuler 
        return false; 
    }
}
```