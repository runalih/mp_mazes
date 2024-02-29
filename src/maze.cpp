/* Your code here! */
#include "maze.h"
#include "cs225/PNG.h"
#include "dsets.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <utility>
#include <vector>

using namespace std;
using namespace cs225;

SquareMaze::SquareMaze()
{
    width_ = 0;
    height_ = 0;
}
void SquareMaze::makeMaze(int width, int height)
{
    maze_ = vector<Cell>(width * height, Cell());
    width_ = width;
    height_ = height;
    DisjointSets sets;
    int size = width_ * height_;

    // create dsets of width*height
    sets.addelements(size);

    // create vector of wall indices
    vector<int> walls(2 * size);

    // fills walls with indices
    for (int i = 0; i < 2 * size; i++) {
        walls.push_back(i);
    }

    // randomize vector
    std::random_device rdev;
    std::mt19937 randgen(rdev());
    std::shuffle(walls.begin(), walls.end(), randgen);

    for (int wall : walls) {

        int cellIdx = wall / 2;
        int dir = wall % 2;
        int x = getX(cellIdx);
        int y = getY(cellIdx);
        int neighbor = -1;

        //  if right && not perimeter, neighbor = cellIdx + 1
        if (dir == 0) {
            if (x == width_ - 1) continue;
            neighbor = cellIdx + 1;
        }
        //  if down && not perimeter, neighbor = cellIdx + width
        else {
            if (y == height_ - 1) continue;
            neighbor = cellIdx + width_;
        }
        // if 2 cells on side of wall are in same set do nothing
        if (sets.find(cellIdx) == sets.find(neighbor)) continue;

        // joins 2 sets
        sets.setunion(cellIdx, neighbor);

        // removes wall
        setWall(x, y, dir, false);
    }
    // std::cout << "make maze complete!" << std::endl;
}
bool SquareMaze::canTravel(int x, int y, int dir) const
{
    // if wall in given direction, can not travel
    // if going right or down
    if (dir == 0 || dir == 1) {
        // do something
        return !(maze_)[getIndex(x, y)].walls_[dir];
    }
    // if going left
    else if (dir == 2) {
        // if no left neighbor, we are at left edge and can't travel
        if (x == 0) return false;
        // check if left neighbor has a right wall
        return !(maze_)[getIndex(x - 1, y)].walls_[0];
    }
    // if going up
    else if (dir == 3) {
        // if no up neighbor, we are at top edge and can't travel
        if (y == 0) return false;
        // check if up neighbor has a down wall
        return !(maze_)[getIndex(x, y - 1)].walls_[1];
    }
    return false;
}
void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
    // sets maze at coordinate to exists
    (maze_)[getIndex(x, y)].walls_[dir] = exists;
}
vector<int> SquareMaze::solveMaze()
{
    int size = width_ * height_;
    queue<int> q;
    vector<bool> visited(size, false);
    vector<int> distances(size, -1);
    int cellIndex = 0;

    distances[cellIndex] = 0;
    q.push(cellIndex);

    // gets distances of each cell from origin
    while (!q.empty()) {
        cellIndex = q.front();
        q.pop();
        visited[cellIndex] = true;
        for (int neighbor : findNeighbors(cellIndex)) {
            if (visited[neighbor] == false) {
                distances[neighbor] = distances[cellIndex] + 1;
                q.push(neighbor);
            }
        }
    }

    // checks bottom row of distances for destination cell
    int destCell = size - width_;
    for (int i = size - width_; i < size; i++) {
        if (distances[i] > distances[destCell]) {
            destCell = i;
        }
    }

    // find backwards path
    vector<int> solution;
    int index = destCell;

    while (index != 0) {
        for (int neighbor : findNeighbors(index)) {
            if (distances[neighbor] == distances[index] - 1) {
                solution.push_back(findDirection(neighbor, index));
                index = neighbor;
                break;
            }
        }
    }

    reverse(solution.begin(), solution.end());

    return solution;
}
PNG* SquareMaze::drawMaze() const
{
    PNG* png = new PNG(width_ * 10 + 1, height_ * 10 + 1);

    // draw top border
    for (int x = 10; x <= width_ * 10; x++) {
        HSLAPixel& pixel = png->getPixel(x, 0);
        pixel.l = 0;
    }
    // draw left border
    for (int y = 0; y <= height_ * 10; y++) {
        HSLAPixel& pixel = png->getPixel(0, y);
        pixel.l = 0;
    }

    // for every cell, draw right and bottom if can travel
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            // if right wall exists, paint it black
            if (!canTravel(x, y, 0)) {
                for (int k = 0; k <= 10; k++) {
                    HSLAPixel& pixel = png->getPixel((x + 1) * 10, y * 10 + k);
                    pixel.l = 0;
                }
            }
            // if bottom wall exists, paint it black
            if (!canTravel(x, y, 1)) {
                for (int k = 0; k <= 10; k++) {
                    HSLAPixel& pixel = png->getPixel(x * 10 + k, (y + 1) * 10);
                    pixel.l = 0;
                }
            }
        }
    }
    return png;
}
PNG* SquareMaze::drawMazeWithSolution()
{
    PNG* maze = drawMaze();
    vector<int> solution = solveMaze();
    pair<int, int> start(5, 5);
    // getPixel already checks for invalid parameters
    for (int dir : solution) {
        // std::cout << dir << " " << start.first << " " << start.second << std::endl;
        //  going right
        if (dir == 0) {
            for (int p = 0; p < 11; p++) {
                HSLAPixel& pixel = maze->getPixel(start.first, start.second);
                makeRed(pixel);
                start.first++;
            }
            start.first--;
        } // going down
        else if (dir == 1) {
            for (int p = 0; p < 11; p++) {
                HSLAPixel& pixel = maze->getPixel(start.first, start.second);
                makeRed(pixel);
                start.second++;
            }
            start.second--;
        } // going left
        else if (dir == 2) {
            for (int p = 0; p < 11; p++) {
                HSLAPixel& pixel = maze->getPixel(start.first, start.second);
                makeRed(pixel);
                start.first--;
            }
            start.first++;
        } // going up
        else if (dir == 3) {
            for (int p = 0; p < 11; p++) {
                HSLAPixel& pixel = maze->getPixel(start.first, start.second);
                makeRed(pixel);
                start.second--;
            }
            start.second++;
        }
    }
    // start is now our end destination point
    std::cout << start.first << " " << start.second << std::endl;
    for (int k = 1; k <= 9; k++) {
        HSLAPixel& pixel = maze->getPixel(start.first - 5 + k, start.second + 5);
        pixel.l = 1.0;
        // makeRed(pixel);
    }
    return maze;
}
// given x,y coordinates, returns index in maze vector
int SquareMaze::getIndex(int x, int y) const { return x + (width_ * y); }

int SquareMaze::getX(int index) const { return index % width_; }

int SquareMaze::getY(int index) const { return index / width_; }

void SquareMaze::makeRed(HSLAPixel& pixel)
{
    pixel.h = 0;
    pixel.s = 1;
    pixel.l = 0.5;
    pixel.a = 1;
}

vector<int> SquareMaze::findNeighbors(int index)
{
    vector<int> neighbors;

    // neighbor to the right
    if (canTravel(getX(index), getY(index), 0)) neighbors.push_back(index + 1);
    // neighbor down
    if (canTravel(getX(index), getY(index), 1)) neighbors.push_back(index + width_);
    // neighbor left
    if (canTravel(getX(index), getY(index), 2)) neighbors.push_back(index - 1);
    // neighbor up
    if (canTravel(getX(index), getY(index), 3)) neighbors.push_back(index - width_);

    return neighbors;
}

int SquareMaze::findDirection(int from, int to)
{
    // right direction
    if (from + 1 == to) return 0;
    // down direction
    if (from + width_ == to) return 1;
    // left direction
    if (from - 1 == to) return 2;
    // up direction
    if (from - width_ == to) return 3;
    return -1;
}

void SquareMaze::show(vector<int> vect)
{
    for (int i = 0; i < (int)vect.size(); i++) {
        if (i % width_ == 0) {
            std::cout << std::endl;
        }
        std::cout << vect[i] << " ";
    }
}