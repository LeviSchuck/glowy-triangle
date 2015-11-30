#pragma once
#include <queue>


struct PrioritizedPoint {
    int x, y;
    float priority;
    float cost;
    bool operator<(PrioritizedPoint other) const {
        return priority > other.priority;
    }
};

class AStarGrid {
public:
    AStarGrid();
    ~AStarGrid();
    void init(int,int);
    void cleanUp();
    void setValue(int,int,float);
    void setInfinityAt(int,int);
    bool step();
    bool pathStep();
    float valueAt(int,int);
    float topologyAt(int,int);
    float costAt(int,int);
    void setOrigin(int,int);
    void setDestination(int,int);
    float hueristicTo(int,int);
    bool isInfiniteAt(int,int);
    bool isOrigin(int,int);
    bool isDestination(int,int);
    bool isPathAt(int,int);
    void clear(int,int);
private:
    void setTopology(int,int,float);
    void setCost(int,int,float);
    float * grid;
    float * topology;
    float * costs;
    bool * path;
    int width;
    int height;
    int origin_x,origin_y;
    int dest_x,dest_y;
    std::priority_queue<PrioritizedPoint> points;
    std::priority_queue<PrioritizedPoint> paths;
};

