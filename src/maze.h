/* Your code here! */
#pragma once

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "dsets.h"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using namespace cs225;

class SquareMaze {
public:
    SquareMaze();
    //~SquareMaze();
    void makeMaze(int width, int height);
    bool canTravel(int x, int y, int dir) const;
    void setWall(int x, int y, int dir, bool exists);
    vector<int> solveMaze();
    PNG* drawMaze() const;
    PNG* drawMazeWithSolution();
    int getIndex(int x, int y) const;
    int getX(int index) const;
    int getY(int index) const;
    void makeRed(HSLAPixel& pixel);
    vector<int> findNeighbors(int index);
    int findDirection(int from, int to);

private:
    class Cell {
    public:
        Cell() { walls_ = vector<bool>(2, true); }
        vector<bool> walls_;
    };
    vector<Cell> maze_;
    int width_;
    int height_;
    void show(vector<int> vect);
};