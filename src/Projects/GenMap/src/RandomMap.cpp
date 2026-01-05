#include "RandomMap.h"

#include <algorithm>


const CellList RegionBuild::evenNeighbours({{-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}});
const CellList RegionBuild::oddNeighbours({{-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1}});

Grid::Grid(int rows,int cols) : nrow(rows), ncol(cols) {
            tab.resize(nrow, std::vector<int>(ncol, 0));
        }
Grid::~Grid() = default;

int Grid::getCell(int x, int y) {
    return (x >= 0 && x < nrow && y >= 0 && y < ncol) ? tab[x][y] : -1;
}

void Grid::setCell(int x, int y, int value, bool force) {
    if (x >= 0 && x < nrow && y >= 0 && y < ncol && (tab[x][y] == 0 || force))
        tab[x][y] = value;
}

CellList::CellList() = default;
CellList::~CellList() = default;
CellList::CellList(std::initializer_list<std::pair< int,  int>> init) {
    for (auto& cell : init) {
        cells.push_back(cell);
    }
}

void CellList::add(const std::pair<int, int>& cell) {
    cells.push_back(cell);
}

void CellList::remove(const std::pair<int, int>& cell) {
    cells.erase(std::remove(cells.begin(), cells.end(), cell), cells.end());
}

auto CellList::begin() { return cells.begin(); }
auto CellList::end() { return cells.end(); }

auto CellList::begin() const { return cells.begin(); }
auto CellList::end() const { return cells.end(); }

std::pair<int, int> CellList::GetRandomCell() {
    if (cells.empty()) {
        return std::pair<int, int>(-1, -1);
    }
    int randomIndex = rand() % cells.size();
    std::pair<int, int> randomCell = cells[randomIndex];
    cells.erase(cells.begin() + randomIndex);
    return randomCell;
}

bool CellList::contains(const std::pair< int,  int>& cell) const {
    for (const auto& c : cells) {
        if (c == cell) {
            return true;
        }
    }
    return false; 
}

std::vector<std::pair<unsigned int, unsigned int>> CellList::toUnsignedInt() const {
    std::vector<std::pair<unsigned int, unsigned int>> unsignedCells;
    
    for (const auto& cell : cells) {
        unsignedCells.push_back({static_cast<unsigned int>(cell.first), static_cast<unsigned int>(cell.second)});
    }
    
    return unsignedCells;
}




RegionBuild::RegionBuild(int _r, int _c, float _maxSize, int _x, int _y, Grid* _grid): r(_r), c(_c), maxSize(_maxSize), grid(_grid) {
            center = {_x, _y};
            addCell(center);
            computeBorder(center);
            parent = nullptr;
        }
        
RegionBuild::~RegionBuild() = default;

void RegionBuild::setGridCell(int x, int y, int value) {
    grid->setCell(x, y, value);
}

RegionBuild* RegionBuild::Find() {
    if (parent == nullptr) return this;
    parent = parent->Find();
    return parent;
}

void RegionBuild::Union(RegionBuild* region) {
    RegionBuild* root1 = Find();
    RegionBuild* root2 = region -> Find();
    if (root1 != root2) {
        root1->parent = root2;
    }
}
        
void RegionBuild::addCell(std::pair<int, int> cell) {
    cells.add(cell);
    setGridCell(cell.first,cell.second,id);
}

float RegionBuild::distance(std::pair<int, int> cell1, std::pair<int, int> cell2) {
    int distx = cell2.first - cell1.first;
    int disty = cell2.second - cell1.second;
    return std::sqrt(distx * distx + disty * disty);
}

void RegionBuild::enlargeBorder() {
    std::pair<int, int> newcell = border.GetRandomCell();
    if (grid->getCell(newcell.first,newcell.second) != 0) return;
    if (cells.contains(newcell)) return;
    if (newcell == std::pair<int, int>{-1, -1} || distance(newcell, center) > maxSize) return;

    computeBorder(newcell);
    addCell(newcell);
}

void RegionBuild::computeBorder(std::pair<int, int> cell) {
    for (const auto& neighbour : ((cell.first % 2 == 0) ? evenNeighbours : oddNeighbours)) {
        int new_r = neighbour.first + cell.first;
        int new_c = neighbour.second + cell.second;

        if (new_r >= 0 && new_c >= 0 && new_r < r && new_c < c) {
            if (!cells.contains({new_r, new_c})) {
                border.add({new_r, new_c});
            }
        }
        border.remove(cell);
    }
}

int GenerateRandomMap(std::vector<RegionBuild>& regions,int r, int c, int subdivisionFactor, int offsetFactor, float maxTerritorySize, int iteration, int fillPercent, bool testPrint) {

    //Les origines des territoires sont réparties uniformément sur la grille d'hexagone (avec un léger décalage aléatoire pour chaque centre afin de rendre le tout plus organique). Depuis ces origines, les territoires s'étendent un par un et s'arrête quand ils ne peuvent plus s'étendre (ils ne peuvent pas s'étendre sur un autre territoire et ont un rayon maximum d'expansion). Enfin, on supprime les territoires qui n'ont pas assez de cases et qui ne déconnectent pas le graphe. 
    //sizeX : largeur de la carte (32 sur le site dicewars)
    //sizeY : hauteur de la carte (32 sur le site dicewars)
    //subdivisionFactor : en combien de territoire on découpe la carte (environ 8 sur le site dicewars)
    //offsetFactor : décalage aléatoire du centre par rapport a sa supposée origine (environ 1 sur le site dicewars)
    //maxTerritoryize : rayon maximum d'expansion d'un territoire (environ 4 sur le site dicewars)
    //testPrint : affiche la carte des regions (rouge) et des frontières (bleu) avec les frontières connexes (vert)

    Grid grid(r,c);
    int ecartx = c / subdivisionFactor;
    int ecarty = r / subdivisionFactor;
    int startx = ecartx / 2;
    int starty = ecarty / 2;
    int territoryID = 0;

    for (int x = startx; x < c; x += ecartx) {
        for (int y = starty; y < r; y += ecarty) {
            int isPlacing = (rand() % 100);
            if(isPlacing <= fillPercent) //si la probabilité de placer une région est bonne, continuer.
            {
                int randomOffsetx = (rand() % (1 + offsetFactor * 2)) - offsetFactor;
                int randomOffsety = (rand() % (1 + offsetFactor * 2)) - offsetFactor;
                int xvalue = x + randomOffsetx;
                int yvalue = y + randomOffsety;
                if(grid.getCell(xvalue,yvalue) != 0 || xvalue < 0 || xvalue >= c || yvalue < 0 || yvalue >= r) break;
                territoryID += 1;
                grid.setCell(xvalue,yvalue,territoryID);
                RegionBuild region(r, c, maxTerritorySize, xvalue, yvalue, &grid);
                region.id = territoryID;
                regions.push_back(region);
            }
        }
    }

    for(int i = 0; i< iteration;i++)
    {
		for (auto& region : regions) {
			region.enlargeBorder();
		}
    }

    //Grille d'affichage
    Grid grid2(r,c*2);

    for (auto& r : regions) {
        /////////////////////
        ///UNIR ET TROUVER///
        /////////////////////
        std::set<int> result;
        for (const auto& c : r.border.cells) {
            int neighbourId = grid.getCell(c.first, c.second);
            if (neighbourId > 0 && neighbourId != r.id) {
                result.insert(neighbourId);
            }
        }
        
        //opération inutilement très couteuse, peut se modifier en stockant des pointeurs vers les regions dans les cellules
        for(auto& i : result){
            for(auto& r2 : regions)
            {
                if(r2.id == i)
                {
                    r.Union(&r2);
                }
            }
        }

        ////////////////////////////
        ///PARTIE AFFICHAGE TESTS///
        ////////////////////////////
        if(testPrint)
        {
            for (auto& c : r.border.toUnsignedInt())
            {
                grid2.setCell(c.first,c.second*2 + (c.first % 2 == 0 ? 0 : 1),2);
            }
            for (auto& c : r.cells.toUnsignedInt())
            {
                int cellx = c.first;
                int celly = c.second*2 + (c.first % 2 == 0 ? 0 : 1);
                if(grid2.getCell(cellx,celly) == 2)
                {
                    grid2.setCell(cellx,celly,3,true); //frontiere + cell
                }
                else if(grid2.getCell(cellx,celly) == 1)
                {
                    grid2.setCell(cellx,celly,4,true); //cell + cell = PAS NORMAL
                }
                else if(grid2.getCell(cellx,celly) == 0)
                {
                    grid2.setCell(cellx,celly,1,true); //rien : cell
                }
            }
        }
    }

    if(testPrint)
    {
    for(int x = 0 ; x < r ; x ++)
        {
            for(int y = 0 ; y < c*2; y ++)
            {
                
                int cell = grid2.getCell(x,y);
                //std::cout << cell;
                if(cell == 1) //cellule
                    std::cout << "\033[1;31m" << "██" << "\033[0m";
                
                else if(cell == 2) //frontiere
                    std::cout << "\033[1;34m" << "██" << "\033[0m";
                
                else if(cell == 3) //frontiere + cellule
                    std::cout << "\033[1;32m" << "██" << "\033[0m";

                else if(cell == 4) // cellule + cellule (pas normal)
                    std::cout << "\033[1;40m" << "██" << "\033[0m";
                else
                    std::cout << "  ";
            }
            std:: cout << std::endl;
        }   
    }

    

    //////////////////////////////////////
    ///GESTION DES COMPOSANTES CONNEXES///
    //////////////////////////////////////
    std::unordered_map<RegionBuild*, int> taillesComposantes;
    for(auto& r : regions)
    {
        taillesComposantes[r.Find()] += 1;
    }
    
    int taillemax=0;
    int rootmax=-1;
    for (const auto& [root, taille] : taillesComposantes) {
        if(taille > taillemax)
        {
            taillemax = taille;
            rootmax = root->id;
        }
    }

    for(auto& r : regions)
    {
        if(r.Find()->id != rootmax)
        {
            for(auto& c : r.cells.cells)
            {
                grid.setCell(c.first,c.second,0);
            }
        }
    }

    return rootmax;
}

using Regions2 = std::vector<RegionBuild>;

void LoadRandomMap(Regions& regions,unsigned int r,unsigned int c)
{    
	regions.clear();
	srand(static_cast<unsigned>(std::time(nullptr)));
    Regions2 region2 = {};
    //Il serait possible de calculer le facteur de subdivision, la taille max des territoires ainsi que le nombre d'itération en fonction
    //de la taille de la grille afin d'avoir un nombre de territoire homogène même avec une grille plus ou moins grande.

    int mapSize = r*c;
    int maximumGenerationTry = 3000;//pour ne pas que l'algo tourne indéfiniment, on définit un nombre d'essai maximum

    //les trois valeurs suivantes ont étées choisies arbitrairement pour avoir une bonne forme de carte, elles sont toujours modifiable.
    int minimumRegionNumber = 29;//on veut au minimum 16 régions
    int maximumRegionNumber = 35;//on veut au maximum 35 régions
    int iteration = mapSize/40;
    int territorySize = mapSize/80;

    int regionAmount = 0;
    int rootMax = 0;
    int tryNumber = 0;
    
    std::cout << "Generating a map with [map size : " << mapSize << " iteration : " << iteration << " territory size : " << territorySize << "]" << std::endl;

    while(tryNumber < maximumGenerationTry && (regionAmount < minimumRegionNumber || regionAmount > maximumRegionNumber))
    {
        int fillPercent = 50 + maximumGenerationTry/(maximumGenerationTry-tryNumber + 1)/2; //on fait varier le pourcentage de remplissage de 50 à 100 en fonction des tests;
        //std::cout << "try : " << tryNumber << std::endl;
        regionAmount = 0;
        region2 = {};

        rootMax = GenerateRandomMap(region2,r,c,8,3,territorySize,iteration,fillPercent);
    
        for (auto& r : region2) {
            if(r.Find()->id != rootMax) continue;
            regionAmount ++;
        }

        tryNumber ++;
    }

    for (auto& r : region2) {
        if(r.Find()->id != rootMax) continue;
    	regions.push_back(r.cells.toUnsignedInt());
    }
}