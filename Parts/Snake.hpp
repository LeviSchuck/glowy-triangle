#pragma once

#include <list>
#include <set>
#include <map>

#include "Parts/Algorithm.hpp"

class Snake : public Algorithm {
public:
    Snake();
    ~Snake();
    virtual void init(Screen*);
    virtual bool step(Screen*);
private:
    void makeFood();
    bool stepSnake();
    std::set<Point> walls;
    std::map<Point,int> tried;
    int tryFactor;
    int width, height;
    Point food;
    std::list<Point> snake;
    
};
