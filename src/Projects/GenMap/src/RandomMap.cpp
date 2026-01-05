#include "RandomMap.h"

class Grid {
    private:
        int ncol;
        int nrow;
        std::vector<std::vector<int>> tab;

    public:
        Grid(int rows,int cols) : nrow(rows), ncol(cols) {
            tab.resize(nrow, std::vector<int>(ncol, 0));
        }
        ~Grid() = default;

        int getCell(int x, int y) {
            return (x >= 0 && x < nrow && y >= 0 && y < ncol) ? tab[x][y] : -1;
        }

        void setCell(int x, int y, int value, bool force = false) {
            if (x >= 0 && x < nrow && y >= 0 && y < ncol && (tab[x][y] == 0 || force))
                tab[x][y] = value;
        }
};

class CellList {
    public:
        std::vector<std::pair< int,  int>> cells;

        CellList() = default;
        ~CellList() = default;
        CellList(std::initializer_list<std::pair< int,  int>> init) {
            for (auto& cell : init) {
                cells.push_back(cell);
            }
        }

        void add(const std::pair<int, int>& cell) {
            cells.push_back(cell);
        }

        void remove(const std::pair<int, int>& cell) {
            cells.erase(std::remove(cells.begin(), cells.end(), cell), cells.end());
        }

        auto begin() { return cells.begin(); }
        auto end() { return cells.end(); }

        auto begin() const { return cells.begin(); }
        auto end() const { return cells.end(); }

        std::pair<int, int> GetRandomCell() {
            if (cells.empty()) {
                return std::pair<int, int>(-1, -1);
            }
            int randomIndex = rand() % cells.size();
            std::pair<int, int> randomCell = cells[randomIndex];
            cells.erase(cells.begin() + randomIndex);
            return randomCell;
        }

        bool contains(const std::pair< int,  int>& cell) const {
            for (const auto& c : cells) {
                if (c == cell) {
                    return true;
                }
            }
            return false; 
        }

        std::vector<std::pair<unsigned int, unsigned int>> toUnsignedInt() const {
            std::vector<std::pair<unsigned int, unsigned int>> unsignedCells;
            
            for (const auto& cell : cells) {
                unsignedCells.push_back({static_cast<unsigned int>(cell.first), static_cast<unsigned int>(cell.second)});
            }
            
            return unsignedCells;
        }


        friend std::ostream& operator<<(std::ostream& os, const CellList& list) {
            os << "[ ";
            for (const auto& cell : list.cells) {
                os << "(" << cell.first << ", " << cell.second << ") ";
            }
            os << "]";
            return os;
        }
};

class RegionBuild {
    private:
        float maxSize;
        std::pair<int, int> center;

    public:
        int id;
        Grid *grid;
        RegionBuild* parent;
        int r;
        int c;
        static const CellList evenNeighbours;
        static const CellList oddNeighbours;
        CellList cells;
        CellList frontiere;

        RegionBuild(int _r, int _c, float _maxSize, int _x, int _y, Grid* _grid): r(_r), c(_c), maxSize(_maxSize), grid(_grid) {
            center = {_x, _y};
            addCell(center);
            calculerFrontiere(center);
            parent = nullptr;
        }
        
        ~RegionBuild() = default;

        void setGridCell(int x, int y, int value) {
            grid->setCell(x, y, value);
        }

        RegionBuild* Find() {
            if (parent == nullptr) return this;
            parent = parent->Find();
            return parent;
        }

        void Union(RegionBuild* region) {
            RegionBuild* root1 = Find();
            RegionBuild* root2 = region -> Find();
            if (root1 != root2) {
                root1->parent = root2;
            }
        }
        
        void addCell(std::pair<int, int> cell) {
            cells.add(cell);
            setGridCell(cell.first,cell.second,id);
        }

        float distance(std::pair<int, int> cell1, std::pair<int, int> cell2) {
            int distx = cell2.first - cell1.first;
            int disty = cell2.second - cell1.second;
            return std::sqrt(distx * distx + disty * disty);
        }

        void aggrandirFrontiere() {
            std::pair<int, int> newcell = frontiere.GetRandomCell();
            if (grid->getCell(newcell.first,newcell.second) != 0) return;
            if (cells.contains(newcell)) return;
            if (newcell == std::pair<int, int>{-1, -1} || distance(newcell, center) > maxSize) return;

            calculerFrontiere(newcell);
            addCell(newcell);
        }

        void calculerFrontiere(std::pair<int, int> cell) {
            for (const auto& voisin : ((cell.first % 2 == 0) ? evenNeighbours : oddNeighbours)) {
                int new_r = voisin.first + cell.first;
                int new_c = voisin.second + cell.second;

                if (new_r >= 0 && new_c >= 0 && new_r < r && new_c < c) {
                    if (!cells.contains({new_r, new_c})) {
                        frontiere.add({new_r, new_c});
                    }
                }
                frontiere.remove(cell);
            }
        }
};

const CellList RegionBuild::evenNeighbours({{-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}});
const CellList RegionBuild::oddNeighbours({{-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1}});

int GenerateRandomMap(std::vector<RegionBuild>& regions, int r = 28, int c = 32, int subdivisionFactor = 6, int offsetFactor = 3, float maxTerritorySize = 5,int iteration = 30,bool testPrint = false) {

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

    for(int i = 0; i< iteration;i++)
    {
		for (auto& region : regions) {
			region.aggrandirFrontiere();
		}
    }

    //Grille d'affichage
    Grid grid2(r,c*2);

    for (auto& r : regions) {
        /////////////////////
        ///UNIR ET TROUVER///
        /////////////////////
        std::set<int> result;
        for (const auto& c : r.frontiere.cells) {
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
            for (auto& c : r.frontiere.toUnsignedInt())
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
	int rootMax = GenerateRandomMap(region2,r,c);

    for (auto& r : region2) {
        if(r.Find()->id != rootMax) continue;
    	regions.push_back(r.cells.toUnsignedInt());
    }
}