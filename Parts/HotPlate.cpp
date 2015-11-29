#include <cfloat>
#include <cmath>
#include <cstdlib>

#include "defs.h"
#include "Parts/HotPlate.hpp"

HotPlate::HotPlate() {
    for(int i = 0; i < 10; i++) {
        locked[i].x = 0;
        locked[i].y = 0;
    }
    pointsLocked = 0;
}

void HotPlate::init(Screen * s) {
    int maxW = s->width;
    int maxH = s->height;
    pointsLocked = (RANDOM() % 9) + 1;
    for(int i = 0; i < pointsLocked; i++) {
        locked[i].x = RANDOM() % maxW;
        locked[i].y = RANDOM() % maxH;
    }
    for(int x = 0; x < s->width; x++) {
        for(int y = 0; y < s->height; y++) {
            s->pixelAt(x,y)->saturation=1.0;
            s->pixelAt(x,y)->hue=1;
        }
    }
}

bool HotPlate::isLocked(int x, int y) {
    if(x<0 || y<0) return true;
    // Not the most efficient way, since it looks
    // over the whole list every time.
    // But I'm aiming for correctness and simplicity
    // for now.
    for(int i = 0; i < pointsLocked; i++) {
        if(locked[i].x==x && locked[i].y==y) return true;
    }
    return false;
}

float HotPlate::valueAt(Screen * s, int x, int y) {
    // cover the edges.
    if(x<0 || y<0 || x >= s->width || y >= s->height) return 0.0;
    // cover the hot positions
    if(isLocked(x,y)) return 1;
    // Use the value listed in the screen pixel
    return s->pixelAt(x,y)->value;
}

bool HotPlate::step(Screen * sc) {
    bool changes = false;
    float newValue = 0;
    float oldValue;
    float n,s,e,w,here;
    float blue;
    float epsilon = 1.0/512.0;
    // use FLT_EPSILON to tell if there's a measureable change
    // in the hotplate
    for(int x = 0; x < sc->width; x++) {
        for(int y = 0; y < sc->height; y++) {
            here = valueAt(sc,x,y);
            n = valueAt(sc,x,y-1);
            s = valueAt(sc,x,y+1);
            e = valueAt(sc,x-1,y);
            w = valueAt(sc,x+1,y);
            newValue = (here+n+s+e+w)/5;
            if(isLocked(x,y)) newValue = here;
            oldValue = sc->pixelAt(x,y)->value;
            if(fabs(newValue - oldValue) > epsilon) changes = true;
            sc->pixelAt(x,y)->value = newValue;
            blue = 360 - ((1.0-newValue) * 60);
            sc->pixelAt(x,y)->hue = blue;

        }
    }
    return changes;
}
