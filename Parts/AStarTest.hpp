#pragma once
#include "Parts/Algorithm.hpp"
#include "Parts/AStar.hpp"

class AStarTest : public Algorithm {
public:
    AStarTest();
    virtual void init(Screen*);
    virtual bool step(Screen*);
private:
    AStarGrid grid;
    bool foundPath;
};
