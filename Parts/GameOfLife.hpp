#pragma once
#include "Algorithm.hpp"

class GameOfLife : public Algorithm {
public:
    GameOfLife();
    ~GameOfLife();
    virtual void init(Screen*);
    virtual bool step(Screen*);
    int countNeighbors(int,int);
    bool isOnAt(int,int);
private:
    void cleanUp();
    bool * grid;
    int width;
    int height;
    int iters;
};
