#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <cmath> 
#include <iostream>
#include <typeinfo>
#include <map>
#include <set>
#include <unordered_map>

#include "MapLoader.h"


void LoadRandomMap(Regions& regions, unsigned int r = 28, unsigned int c = 32);

class Grid {
    private:
        int ncol;
        int nrow;
        std::vector<std::vector<int>> tab;

    public:
        Grid(int rows,int cols);
        ~Grid();
        int getCell(int x, int y);
        void setCell(int x, int y, int value, bool force = false);
};

class CellList {
    public:
        std::vector<std::pair< int,  int>> cells;
        CellList();
        ~CellList();
        CellList(std::initializer_list<std::pair< int,  int>> init);

        void add(const std::pair<int, int>& cell);

        void remove(const std::pair<int, int>& cell);

        auto begin();
        auto end();

        auto begin() const;
        auto end() const;

        std::pair<int, int> GetRandomCell();

        bool contains(const std::pair< int,  int>& cell) const;
        
        std::vector<std::pair<unsigned int, unsigned int>> toUnsignedInt() const;

        friend std::ostream& operator<<(std::ostream& os, const CellList& list) {
        os << "[ ";
        for (const auto& cell : list.cells) {
            os << "(" << cell.first << ", " << cell.second << ") ";
        }
        os << "]";
        return os;
    };
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
        CellList border;

        RegionBuild(int _r, int _c, float _maxSize, int _x, int _y, Grid* _grid);
        
        ~RegionBuild();

        void setGridCell(int x, int y, int value);

        RegionBuild* Find();

        void Union(RegionBuild* region);
        
        void addCell(std::pair<int, int> cell);

        float distance(std::pair<int, int> cell1, std::pair<int, int> cell2);

        void enlargeBorder();

        void computeBorder(std::pair<int, int> cell);
};

int GenerateRandomMap(std::vector<RegionBuild>& regions,int r = 28, int c = 32, int subdivisionFactor = 6, int offsetFactor = 3, float maxTerritorySize = 5,int iteration = 35,  int fillPercent = 100, bool testPrint = false);

using Regions2 = std::vector<RegionBuild>;

void LoadRandomMap(Regions& regions,unsigned int r,unsigned int c);