#pragma once

#include <set>
#include <map>

#include "Parts/Algorithm.hpp"

class HotPlate : public Algorithm {
public:
    HotPlate();
    virtual void init(Screen*);
    virtual bool step(Screen*);
    bool isLocked(Point);
    void setLock(Point);
    float valueAt(Point,int,int);
    bool stepHP();
    void reset();
    void setDims(int,int);
private:
    std::set<Point> locked;
    std::map<Point,float> temp1,temp2;
    bool swap;
    int width, height;

};
