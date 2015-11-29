#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#include "defs.h"
#include "Parts/GameOfLife.hpp"

GameOfLife::GameOfLife(){
    iters = 0;
    grid = 0;
}
GameOfLife::~GameOfLife(){
    if(grid) delete[] grid;
}
void GameOfLife::init(Screen * s) {
    if(grid) delete[] grid;
    int maxgrid = s->width * s->height;
    iters = 0;
    grid = new bool[maxgrid];
    width = s->width;
    height = s->height;

    // Now scramble the grid.
    for(int i = 0; i < maxgrid; i++) {
        if(RANDOM() % 2 == 1) grid[i] = true;
        else grid[i] = false;
    }
}

bool GameOfLife::isOnAt(int x, int y){
    if(x < 0 || y < 0) return false;
    if(x >= width || y >= height) return false;
    int pos = width * y + x;
    return grid[pos];
}

int GameOfLife::countNeighbors(int x, int y) {
    int count = 0;
    for(int i = x-1; i <= x+1; i++) {
        for(int j = y-1; j <= y+1; j++) {
            if(i == x && j == y) continue;
            if(isOnAt(i,j)) count++;
        }
    }
    return count;
}

bool GameOfLife::step(Screen * sc) {
    int pos,count;
    iters++;
    bool changes = false;

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++){
            pos = width * y + x;
            count = countNeighbors(x,y);
            if(isOnAt(x,y)){
                // cases for cells to die
                if(count < 2) {
                    // under population
                    grid[pos] = false;
                    changes = true;
                } else if(count > 3) {
                    // over population
                    grid[pos] = false;
                    changes = true;
                }
            } else if(count == 3){
                // Dead cell becomes alive
                grid[pos] = true;
                changes = true;
            }
            // set screen values
            if(isOnAt(x,y)){
                sc->pixelAt(x,y)->value = 1;
            } else {
                sc->pixelAt(x,y)->value = 0;
            }
        }
    }

    if(iters>1000 || !changes) return false;
    return true;
}
