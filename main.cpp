#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <unistd.h>

#include "defs.h"
#include "color.h"
#include "Parts/Screen.hpp"
#include "Parts/Algorithm.hpp"
#include "Parts/HotPlate.hpp"
#include "Parts/GameOfLife.hpp"
#include "Parts/AStarTest.hpp"
#include "Parts/Snake.hpp"




void putTermColor(Screen * s) {
    float r,g,b;
    for(int x = 0; x < s->width; x++) {
        for(int y = 0; y < s->height; y++) {
            Pixel * p = s->pixelAt(x,y);
            HSVtoRGB(&r,&g,&b,p->hue,p->saturation,p->value);
            colorSpace(r*255,g*255,b*255);
        }
        colorSpace(0,0,0);
        printf("\n");
    }
    printf("\n");
}

int main(){
    int alg;
    SEEDRANDOM;
    while(true) {
        printf("New Try\n");
        Screen s(10,10);
        Algorithm * a;
        //Algorithm * a = new HotPlate();
        //Algorithm * a = new GameOfLife();
        // Randomly select an algorithm
        alg = RANDOM() % 4;
        alg = 9999; // Fix to current one
        switch(alg) {
            case 0: a = new HotPlate(); break;
            case 1: a = new GameOfLife(); break;
            case 2: a = new AStarTest(); break;
            case 3: a = new Snake(); break;

            default: a = new Snake(); break;
        }

        a->init(&s);
        do {
            putTermColor(&s);
            usleep(40 * 1000);
            //sleep(1);
        } while(a->step(&s));
        putTermColor(&s);
        sleep(1);
        delete a;
    }
}


