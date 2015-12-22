#pragma once
#include "Parts/Algorithm.hpp"
#include "Parts/AStar.hpp"

class AStarTest : public Algorithm {
public:
    AStarTest();
    virtual void init(Screen*);
    virtual bool step(Screen*);
    virtual int stepDelay() {
      return 60;
    }
private:
    AStarGrid grid;
    bool foundPath;
};
