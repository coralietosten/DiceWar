#include "RandomMap.h"
// Définir un alias pour les régions
class Grid {
    private:
        int ncol;
        int nrow;
        std::vector<std::vector<int>> tab;

    public:
        Grid(int rows,int cols) : nrow(rows), ncol(cols) {
            tab.resize(nrow, std::vector<int>(ncol, 0));
        }

        int getCell(int x, int y) {
            return (x >= 0 && x < nrow && y >= 0 && y < ncol) ? tab[x][y] : -1;
        }

        void setCell(int x, int y, int value) {
            if (x >= 0 && x < nrow && y >= 0 && y < ncol)
                tab[x][y] = value;
        }
};

class CellList {
public:
    std::vector<std::pair< int,  int>> cells;

    CellList() = default;

    CellList(std::initializer_list<std::pair< int,  int>> init) {
        for (auto& cell : init) {
            cells.push_back(cell);
        }
    }

    void add(const std::pair<int, int>& cell) {
        cells.push_back(cell);
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
    Grid &grid;
    int parentId;
    int r;
    int c;
    CellList evenNeighbours;
    CellList oddNeighbours;
    CellList cells;
    CellList frontiere;

    void setGridCell(int x, int y, int value) {
        grid.setCell(x, y, value);
    }

    RegionBuild(int _r, int _c, float _maxSize, int _x, int _y, Grid& _grid)
        : r(_r), c(_c), maxSize(_maxSize), grid(_grid) {
        evenNeighbours = CellList({{-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}});
        oddNeighbours = CellList({{-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1}});
        center = {_x, _y};
        addCell(center);
        calculerFrontiere(center);
        parentId = 0;
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
        if (grid.getCell(newcell.first,newcell.second) != 0) return;
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
        }
    }
    std::set<int> getNeigbour(){
        std::set<int> set;
        for (auto& cell : frontiere.cells) // Convertie en solution 3 à la compilation ?
        {
            set.insert(grid.getCell(cell.first,cell.second));
        }
        return set;
    }
};

void GenerateRandomMap(std::vector<RegionBuild>& regions, int r = 28, int c = 32, int subdivisionFactor = 6, int offsetFactor = 3, float maxTerritorySize = 5,int iteration = 30) {
	
	Grid grid(r,c);
    int ecartx = c / subdivisionFactor;
    int ecarty = r / subdivisionFactor;
    int startx = ecartx / 2;
    int starty = ecarty / 2;
    int territoryID = 1;

    for (int x = startx; x < c; x += ecartx) {
        for (int y = starty; y < r; y += ecarty) {
            int randomOffsetx = (rand() % (1 + offsetFactor * 2)) - offsetFactor;
            int randomOffsety = (rand() % (1 + offsetFactor * 2)) - offsetFactor;
            int xvalue = x + randomOffsetx;
            int yvalue = y + randomOffsety;
            if(grid.getCell(xvalue,yvalue) != 0 || xvalue < 0 || xvalue >= c || yvalue < 0 || yvalue >= r) break;
            territoryID += 1;
            grid.setCell(xvalue,yvalue,territoryID);
			RegionBuild region(r, c, maxTerritorySize, xvalue, yvalue, grid);
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

    ///////////////////////////////////////
    //UNIR ET TROUVER
    ///////////////////////////////////////

    //UnionFindSystem uf;
    //uf.Find(regions,0);
    return;
}

using Regions2 = std::vector<RegionBuild>;

void LoadRandomMap(Regions& regions,unsigned int r,unsigned int c)
{    
	regions.clear();
	srand(static_cast<unsigned>(std::time(nullptr)));
    Regions2 region2 = {};
	GenerateRandomMap(region2,r,c);
    for (const auto& region : region2) {
		//std::cout << region.cells << std::endl;
    	regions.push_back(region.cells.toUnsignedInt());
    }
}

//Les origines des territoires sont réparties uniformément sur la grille d'hexagone (avec un léger décalage aléatoire pour chaque centre afin de rendre le tout plus organique). Depuis ces origines, les territoires s'étendent un par un et s'arrête quand ils ne peuvent plus s'étendre (ils ne peuvent pas s'étendre sur un autre territoire et ont un rayon maximum d'expansion). Enfin, on supprime les territoires qui n'ont pas assez de cases et qui ne déconnectent pas le graphe. 
//sizeX : largeur de la carte (32 sur le site dicewars)
//sizeY : hauteur de la carte (32 sur le site dicewars)
//subdivisionFactor : en combien de territoire on découpe la carte (environ 8 sur le site dicewars)
//offsetFactor : décalage aléatoire du centre par rapport a sa supposée origine (environ 1 sur le site dicewars)
//maxTerritoryize : rayon maximum d'expansion d'un territoire (environ 4 sur le site dicewars)

class UnionFindSystem{
    public : 
        UnionFindSystem() = default;
        int Find(std::vector<RegionBuild>& regions, int regionId)
        {
            RegionBuild& region = regions[regionId];
            if(region.parentId == 0)
            {
                return region.id;
            }
            else
            return Find(regions,region.parentId);
        }

        void Union(std::vector<RegionBuild>& regions, int regionId1, int regionId2)
        {
            int root1 = Find(regions,regionId1);
            int root2 = Find(regions,regionId2);
            if(root1 != root2)
            {
                regions[root1].parentId = regions[root2].id;
            }
        }

        void UnionFind(std::vector<RegionBuild>& regions)
        {
            for (int regionId = 0; regionId < regions.size(); regionId++)
            {
                std::set<int> neighbours = regions[regionId].getNeigbour();
                for (int neighbourId : neighbours)
                {
                    Union(regions, regionId, neighbourId);
                }
            }
        }


        std::vector<std::set<int>> ToSets(std::vector<RegionBuild>& regions)
        {
            std::vector<std::set<int>> result;
            std::map<int, std::set<int>> dico;

            for (int regionId = 0; regionId < regions.size(); ++regionId)
            {
                int rootId = Find(regions, regionId);
                dico[rootId].insert(regionId);
            }

            for (const auto& entry : dico)
            {
                result.push_back(entry.second);
            }
            return result;
        }
};