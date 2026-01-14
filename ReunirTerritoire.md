Ce code utilise le chemin renvoyé par ReunirTerritoire.md

```
#include "ReunirTerritoire.h"
#include "probabilite.hpp" 
#include <iostream> 

bool ReunirTerritoire(ResultatChemin& resultat, const SMap& map, STurn& turn)
{

    // Il faut au moins Depart, Arrivee  -> il faudra supprimer le chemin
    if (resultat.chemin.size() < 2) {
        return false; 
    }

    SCell first_owned = resultat.chemin[0]; // Le début du chemin
    SCell last_owned = resultat.chemin.back(); // La fin du chemin

    SCell target_from_start = resultat.chemin[1]; 
    SCell target_from_end = resultat.chemin[resultat.chemin.size() - 2];

    float probability_un = probabilite_victoire(first_owned.infos.nbDices, target_from_start.infos.nbDices); 
    float probability_deux = probabilite_victoire(last_owned.infos.nbDices, target_from_end.infos.nbDices);


    if(target_from_start.infos.owner == first_owned.infos.owner) // si erreur dans le chemin
    {
        // C'est à nous -> Inutile d'attaquer. 
        // On l'enlève du chemin 
        resultat.chemin.erase(resultat.chemin.begin());
        
        return ReunirTerritoire(resultat, map, turn);
    }
    if(target_from_end.infos.owner == last_owned.infos.owner)
    {
        // C'est à nous -> Inutile d'attaquer. 
        // On l'enlève du chemin 
        resultat.chemin.pop_back();
        
        return ReunirTerritoire(resultat, map, turn);
    }


    if(probability_un == 0.0 && probability_deux == 0.0)  // todo : modifier pour ne pas attaquer si chance inférieur à x% ?
    {
        // Aucune chance de réussir depuis les deux côtés
        return false; 
    }

    if (probability_un == 0.0)
    {
        // on fait Astar depuis la fin
        resultat = AStar(map, last_owned, first_owned, resultat.chemin);

        return ReunirTerritoire(resultat, map, turn);
    }



    if (probability_un >= probability_deux)
    {
         // On attaque depuis le début car c'est là qu'on a le plus de chances de réussir

        turn.cellFrom = first_owned.infos.id;
        turn.cellTo = target_from_start.infos.id;
        return true; 
    }

    else
    {

        turn.cellFrom = last_owned.infos.id;
        turn.cellTo = target_from_end.infos.id;
        return true;

    }
}
```

voici le .h

```
#pragma once
#include "IStrategyLib.h"
#include <vector>
#include "AStar2.h"

// Fonction pour réunir deux territoires en utilisant l'algorithme A*
bool ReunirTerritoire (ResultatChemin& resultat, const SMap& map, STurn& turn);

// si on recoit false alors il faut supprimer le chemin
```