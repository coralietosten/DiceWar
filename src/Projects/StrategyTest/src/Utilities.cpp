#include "Utilities.h"



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


    if(probability_un <=35 && probability_deux <= 35)  // todo : modifier pour ne pas attaquer si chance inférieur à x% ?
    {
        // Aucune chance de réussir depuis les deux côtés
        return false; 
    }

    if (probability_un <= 30)
    {
        // on fait Astar depuis la fin
        resultat = AStar(map, last_owned, first_owned);

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


bool AttackOrNot(const SMap& map, std::vector<std::vector<SCell>>& lsClusters)
{

    if (lsClusters.empty() || lsClusters[0].empty()) return false;
    unsigned int mon_id = lsClusters[0][0].infos.owner;
    bool attaque = true;



    // LOGIQUE 1 : EST ON TROP FAIBLE POUR ATTAQUER ?

    double somme_nos_des = 0.0;
    int nombre_nos_cellules = 0;

    for (const auto& region : lsClusters) {
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
            attaque = false; 
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
    for (auto it = compte_territoires.begin(); it != compte_territoires.end(); ) {
        if (it->second == 0) {
            it = compte_territoires.erase(it);
        } else {
            ++it;
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
    bool condition_monde = (nb_joueurs_actifs < 3);   // si y'a que 2 joueurs = on est en duel il faut attaquer
    if (condition_monde) 
    {
        return true; // en duel, on attaque tout le temps
    }


    if(attaque == false)
    {
        return false; // on est trop faible, on n'attaque pas
    }

    double coefficient_domination = 1.3; // il faudra surment l'ajuster
    
    bool condition_domination = (nombre_nos_territoires > (max_territoires_ennemi * coefficient_domination));
    bool condition_force = (notre_moyenne >= 7.0);  // on remet au cas où

    if (condition_domination && condition_force) 
    {
        return true; // Tous les feux sont au vert, on écrase tout !
    }
    else 
    {
        // Si une seule condition manque (trop faibel ou pas assez dominant), on reste défensif pour accumuler 
        return false; 
    }
}


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

std::optional<Connection> SCellToConnect (const SMap& map, unsigned int player){
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
            return connections[i-1];
        }
    }
    return std::nullopt;
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