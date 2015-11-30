#include <cstdlib>

#include "defs.h"
#include "Parts/AStarTest.hpp"

AStarTest::AStarTest() {

}

void AStarTest::init(Screen * sc) {
    grid.init(sc->width, sc->height);
    foundPath = false;

    Point p;

    int walls = (RANDOM() % 15) + 1;
    for(int i = 0; i < walls; i++) {
        p.x = RANDOM() % sc->width;
        p.y = RANDOM() % sc->height;
        grid.setInfinityAt(p.x,p.y);
    }

    // Set Origin
    p.x = RANDOM() % sc->width;
    p.y = RANDOM() % sc->height;
    grid.setOrigin(p.x,p.y);
    grid.clear(p.x,p.y);

    // Set Destination
    p.x = RANDOM() % sc->width;
    p.y = RANDOM() % sc->height;
    grid.setDestination(p.x,p.y);
    grid.clear(p.x,p.y);
}

bool AStarTest::step(Screen * sc) {
    bool res;
    if(foundPath) {
        res = grid.pathStep();
    } else {
        if(!grid.step()) foundPath = true;
        res = true;
    }

    float cost;
    for(int x = 0; x < sc->width; x++) {
        for(int y = 0; y < sc->height; y++) {
            cost = grid.topologyAt(x,y);
            Pixel * p;
            p = sc->pixelAt(x,y);
            if(grid.isInfiniteAt(x,y)){
                p->value = 0.5;
                p->saturation = 0;
            } else if(grid.isOrigin(x,y)) {
                p->value = 1;
                p->saturation = 0;
            } else if(grid.isDestination(x,y)) {
                p->value = 1;
                p->saturation = 0;
            } else if(grid.isPathAt(x,y)) {
                p->value = 1;
                p->saturation = 0;
            } else if(cost == 0) {
                p->value = 0;
                p->saturation = 0;
                p->hue = 0;
            } else {
                p->value = 1;
                p->saturation = 1;
                p->hue = cost * 20.0;
            }
        }
    }

    return res;
}





