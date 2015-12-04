#include <cfloat>
#include <cmath>
#include <cstdlib>

#include "defs.h"
#include "Parts/HotPlate.hpp"

HotPlate::HotPlate() {
    swap = false;
}

void HotPlate::init(Screen * s) {
    width = s->width;
    height = s->height;
    
    for(int x = 0; x < s->width; x++) {
        for(int y = 0; y < s->height; y++) {
            Point p(x,y);
            temp1[p] = 0;
            temp2[p] = 0;
        }
    }

    int pointsLocked;
    pointsLocked = (RANDOM() % 9) + 1;
    for(int i = 0; i < pointsLocked; i++) {
        Point lock;
        lock.x = RANDOM() % width;
        lock.y = RANDOM() % height;
        locked.insert(lock);
        temp1[lock] = 1.0;
        temp2[lock] = 1.0;
    }
    for(int x = 0; x < s->width; x++) {
        for(int y = 0; y < s->height; y++) {
            s->pixelAt(x,y)->saturation = 1.0;
            s->pixelAt(x,y)->hue = 1.0;
            s->pixelAt(x,y)->value = 0.0;
        }
    }
}

void HotPlate::reset() {
    locked.clear();
    temp1.clear();
    temp2.clear();
}
void HotPlate::setDims(int w, int h) {
    width = h;
    height = h;
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            Point p(x,y);
            temp1[p] = 0;
            temp2[p] = 0;
        }
    }
}

bool HotPlate::isLocked(Point p) {
    return locked.find(p) != locked.end();
}

float HotPlate::valueAt(Point p, int offset_x, int offset_y) {
    Point np = p;
    np.x += offset_x;
    np.y += offset_y;
    // cover the edges.
    if(np.x<0 || np.y<0 || np.x >= width || np.y >= height) {
        if(p.x != np.x && p.y != np.y) {
            // If we are diagonal, let's try to get the point on the same
            // axis.
            if(np.y < 0) {
                return valueAt(p,offset_x,offset_y+1);
            } else if(np.y >= height) {
                return valueAt(p,offset_x,offset_y-1);
            } else if(np.x < 0) {
                return valueAt(p,offset_x+1,offset_y);
            } else if(np.x >= width) {
                return valueAt(p,offset_x-1,offset_y);
            }
        }
        // Arbitrary factor of 1.5 on edges
        if(swap) return temp2[p]/1.5;
        else return temp1[p]/1.5;
    }
    // cover the hot positions
    if(isLocked(np)) return 1;
    // Use the value listed in the screen pixel
    if(swap) return temp2[np];
    else return temp1[np];
}

void HotPlate::setLock(Point p) {
    locked.insert(p);
    temp1[p] = 1;
    temp2[p] = 1;
}

bool HotPlate::stepHP() {
    float newValue;
    float n,s,e,w,here;
    bool changes = false;
    float epsilon = 1.0/512.0;
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            Point p(x,y);

            here = valueAt(p,0,0);
            n = valueAt(p, 0,-1);
            s = valueAt(p, 0, 1);
            e = valueAt(p,-1, 0);
            w = valueAt(p, 1, 0);
            newValue = (here+n+s+e+w)/5;

            if(isLocked(p)) newValue = here;
            if(fabs(newValue - here) > epsilon) changes = true;

            if(swap) temp2[p] = newValue;
            else temp1[p] = newValue;
        }
    }
    swap = !swap;
    return changes;
}

bool HotPlate::step(Screen * sc) {
    bool changes = false;
    float color;
    float intensity;
    // use FLT_EPSILON to tell if there's a measureable change
    // in the hotplate
    changes = stepHP();

    for(int x = 0; x < sc->width; x++) {
        for(int y = 0; y < sc->height; y++) {
            float v;
            Point p(x,y);
            if(swap) v = temp1[p];
            else v = temp2[p];

            sc->pixelAt(x,y)->value = v;
            color = 360 - (1.0-v) * 45;
            intensity = 1 - (v * v);
            sc->pixelAt(x,y)->hue = color;
            sc->pixelAt(x,y)->saturation = intensity;
        }
    }
    return changes;
}
