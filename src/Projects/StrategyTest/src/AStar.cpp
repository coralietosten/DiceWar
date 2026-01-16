#include "AStar.h"






void _AStar(SCell* current, const SCell* goal, std::vector<SCell>& path, double current_prob, int current_dice, std::vector<int>& visited_ids,double& g_max_prob,std::vector<SCell>& g_best_path ) {
    // Cas de Terminaisons de la récursion :

    // STOP : On ne peut pas attaquer avec 1 seul dé 
    if (current_dice <= 1) {
        return; 
    }

 //  on est arrivé à la case d'arrvee
    if (current->infos.id == goal->infos.id) {
        path.push_back(*current);

        // Est-ce que ce chemin est meilleur que celui qu'on avait avant ?
        if (current_prob > g_max_prob) {
            g_max_prob = current_prob; // On sauvegarde la nouvelle meilleure proba
            g_best_path = path;        // On sauvegarde le chemin complet
        }
        
        // si non on fait rien

        // On retire la dernière case avant de quitter (Backtracking)

    // on "efface" nos traces pour permettre d'autres chemins futurs de passer par ici.
        path.pop_back();
        return;
    }


    // algorithme A* récursif :
    
    // On marque la case actuelle comme visitée et on l'ajoute au chemin
    visited_ids.push_back(current->infos.id);
    path.push_back(*current);

    // On regarde tous les voisins
    for (unsigned int i = 0; i < current->nbNeighbors; ++i) {
        SCell* neighbor = current->neighbors[i];
        
        // premiere etape : Vérifier si on est déjà passé par là (pour éviter de tourner en rond)
        bool already_visited = false;
        for (int id : visited_ids) {
            if (id == neighbor->infos.id)
            {
                already_visited = true; break; 
            }
        }
        if (already_visited) continue; // Si visité, on passe au voisin suivant 
        //formule donnée par l'IA, explication :
        /*La traduction littérale

En C++, le mot-clé continue signifie : "Arrête tout ce que tu fais pour cette itération de la boucle, et passe direct au suivant."

Sans le continue : Le code en dessous s'exécuterait (calcul de proba, appel récursif...).

Avec le continue : Le code saute immédiatement à la fin de l'accolade de la boucle for pour passer au voisin i+1.
*/

        if(neighbor->infos.owner == current->infos.owner)
        {
            // C'est à nous -> On ne peut pas attaquer notre propre cellule
            continue; 
        }
        
        // etape 2 : Calculer la probabilité de gagner contre ce voisin

        // Attaquant : current_dice
        // Défenseur : neighbor->infos.nbDices
        double chance_victoire = probabilite_victoire(current_dice, neighbor->infos.nbDices);

        // Si c'est presque impossible (>1%) (exemple 2 vs 8), inutile d'essayer
        if (chance_victoire <= 0.01) continue;



        //  Mettre à jour la probabilité cumulée
        double new_prob = current_prob * (chance_victoire / 100.0);

        // Enfin, : APPEL RÉCURSIF
        
        _AStar(neighbor, goal, path, new_prob, current_dice - 1, visited_ids,g_max_prob,g_best_path);
    }


    // on "efface" nos traces pour permettre d'autres chemins futurs de passer par ici.
    visited_ids.pop_back();
    path.pop_back();
}



// MODIFICATION ICI : On change le type de retour std::vector<SCell> par ResultatChemin // fait par une IA
ResultatChemin AStar(const SMap map, const SCell& start, const SCell& goal) {
    
    // 1. Initialisation des variables 
    // Variables globales

    double g_max_prob = -1.0;                // La meilleure probabilité trouvée jusqu'ici
    std::vector<SCell> g_best_path;          // Le meilleur chemin correspondant
    g_best_path.clear();


    // 2. Préparation des outils pour la récursion
    std::vector<SCell> current_path;   // Le chemin qu'on est en train de tester
    std::vector<int> visited_ids;      // Liste des IDs pour ne pas tourner en rond
    

    // Ici, on cast pour matcher la signature de _AStar qui prend des pointeurs // IA
    SCell* pStart = const_cast<SCell*>(&start); 

    // 3. Lancement de la recherche
    // Probabilité de départ = 1.0 (100%)
    // Dés de départ = start.infos.nbDices
    _AStar(pStart, &goal, current_path, 1.0, start.infos.nbDices, visited_ids,g_max_prob,g_best_path);


    ResultatChemin resultat;
    resultat.chemin = g_best_path;
    resultat.probabilite = g_max_prob;

    // 4. On retourne le meilleur chemin trouvé
    // MODIFICATION ICI : On retourne les deux variables globales dans la structure
    return resultat;
}
