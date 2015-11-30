#include <limits>
#include <cstdlib>

#include "Parts/AStar.hpp"

AStarGrid::AStarGrid() {
    grid = 0;
    topology = 0;
    path = 0;
    costs = 0;
}

void AStarGrid::init(int w, int h) {
    cleanUp();
    width = w;
    height = h;
    grid = new float[w*h];
    topology = new float[w*h];
    path = new bool[w*h];
    costs = new float[w*h];
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            grid[j*w + i] = 0;
            topology[j*w + i] = 0;
            path[j*w +i] = false;
            costs[j*w +i] = 0;
        }
    }
}

AStarGrid::~AStarGrid(){
    cleanUp();
}

void AStarGrid::cleanUp(){
    if(grid) delete[] grid;
    grid = 0;
    if(topology) delete[] topology;
    topology = 0;
    if(path) delete[] path;
    path = 0;
    if(costs) delete[] costs;
    costs = 0;
}


void AStarGrid::setValue(int x, int y, float val) {
    if(x<0 || y<0 || x>=width || y>=height) return;

    grid[y*width + x] = val;
}
void AStarGrid::setInfinityAt(int x, int y) {
    setValue(x,y,std::numeric_limits<float>::infinity());
    setTopology(x,y,std::numeric_limits<float>::infinity());
}

void AStarGrid::setTopology(int x, int y, float val) {
    if(x<0 || y<0 || x>=width || y>=height) return;

    topology[y*width + x] = val;
}

void AStarGrid::setCost(int x, int y, float val) {
    if(x<0 || y<0 || x>=width || y>=height) return;

    costs[y*width + x] = val;
}

float AStarGrid::valueAt(int x, int y) {
    if(x<0 || y<0 || x>=width || y>=height) return std::numeric_limits<float>::infinity();
    
    return grid[y*width + x];
}

bool AStarGrid::isPathAt(int x, int y) {
    if(x<0 || y<0 || x>=width || y>=height) return false;
    
    return path[y*width + x];
}

bool AStarGrid::isInfiniteAt(int x, int y) {
    if(x<0 || y<0 || x>=width || y>=height) return true;
    
    if(grid[y*width + x] == std::numeric_limits<float>::infinity()) return true;
    if(topology[y*width + x] == std::numeric_limits<float>::infinity()) return true;

    return false;
}

float AStarGrid::topologyAt(int x, int y) {
    if(x<0 || y<0 || x>=width || y>=height) return std::numeric_limits<float>::infinity();
    
    return topology[y*width + x];   
}


float AStarGrid::costAt(int x, int y) {
    if(x<0 || y<0 || x>=width || y>=height) return std::numeric_limits<float>::infinity();
    
    if(isInfiniteAt(x,y)) return std::numeric_limits<float>::infinity();

    return costs[y*width + x];   
}

void AStarGrid::setOrigin(int x, int y) {
    origin_x = x;
    origin_y = y;

    PrioritizedPoint p;
    p.x = x;
    p.y = y;
    p.priority = 0;
    p.cost = 0;

    points.push(p);
}

float AStarGrid::hueristicTo(int x, int y) {
    int dx,dy;
    dx = std::abs(x-dest_x);
    dy = std::abs(y-dest_y);
    return (float)(dx+dy);
}

void AStarGrid::setDestination(int x, int y) {
    dest_x = x;
    dest_y = y;
}

void AStarGrid::clear(int x, int y) {
    if(x<0 || y<0 || x>=width || y>=height) return;
    setValue(x,y,0.0);
    setTopology(x,y,0.0);
    setCost(x,y,0.0);
}

bool AStarGrid::isOrigin(int x, int y) {
    return (x == origin_x && y == origin_y);
}
bool AStarGrid::isDestination(int x, int y) {
    return (x == dest_x && y == dest_y);
}

bool AStarGrid::step() {
    PrioritizedPoint p;
    PrioritizedPoint ns[4];
    if(points.empty()) return false;

    p = points.top();
    points.pop();

    // Check and see if we've already explored this spot.
    if(topologyAt(p.x,p.y) != 0.0) return step();

    // Set value of current spot
    setTopology(p.x,p.y,(p.priority * 2 + hueristicTo(p.x,p.y)) / 3);
    setCost(p.x,p.y,p.cost);

    // Check and see if we've hit an infinite
    // (should never be scheduled unless there
    // is no solution!)
    if(isInfiniteAt(p.x,p.y)) return false;

    // Check and see if we are at the destination
    if(p.x == dest_x && p.y == dest_y) return false;


    // Prepare neighbor points to explore
    for(int i = 0; i < 4; i++) {
        ns[i] = p;
    }
    ns[0].y--;
    ns[1].y++;
    ns[2].x--;
    ns[3].x++;

    // Schedule neighbor points if not explored
    for(int i = 0; i < 4; i++) {
        if(ns[i].x < 0 || ns[i].x >= width) continue;
        if(ns[i].y < 0 || ns[i].y >= height) continue;
        if(topologyAt(ns[i].x,ns[i].y) != 0) continue;

        ns[i].priority = ns[i].cost + hueristicTo(ns[i].x,ns[i].y);
        ns[i].cost+= 1.0;

        points.push(ns[i]);
    }
    return true;
}

bool AStarGrid::pathStep() {
    if(paths.empty() && !isPathAt(dest_x, dest_y)) {
        // Give an initial state
        PrioritizedPoint p;
        p.x = dest_x;
        p.y = dest_y;
        p.priority = 0;
        p.cost = 0;
        paths.push(p);
    }
    if(paths.empty()) return false;

    PrioritizedPoint ns[4];
    PrioritizedPoint segment = paths.top();
    paths.pop();

    if(path[segment.y * width + segment.x]) return pathStep();

    path[segment.y * width + segment.x] = true;

    // We've hit the end.
    if(segment.x == origin_x && segment.y == origin_y) return false;

    // Prepare neighbor points to explore
    for(int i = 0; i < 4; i++) {
        ns[i] = segment;
    }
    ns[0].y--;
    ns[1].y++;
    ns[2].x--;
    ns[3].x++;

    for(int i = 0; i < 4; i++) {
        if(ns[i].x < 0 || ns[i].x >= width) continue;
        if(ns[i].y < 0 || ns[i].y >= height) continue;
        if(topologyAt(ns[i].x,ns[i].y) == 0.0) continue;

        ns[i].priority = costAt(ns[i].x,ns[i].y);
        paths.push(ns[i]);
    }
    return true;
}
