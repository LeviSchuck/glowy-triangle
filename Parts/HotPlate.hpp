#pragma once
#include "Parts/Algorithm.hpp"

class HotPlate : public Algorithm {
public:
    HotPlate();
    virtual void init(Screen*);
    virtual bool step(Screen*);
    bool isLocked(int,int);
    float valueAt(Screen*,int,int);
private:
    Point locked[10];
    int pointsLocked;
};
