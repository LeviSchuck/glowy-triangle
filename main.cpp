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
#include "Parts/CornellBox.hpp"




void putTermColor(Screen * s) {
    float r,g,b;
    printf("\033[1;1H");
    for(int x = 0; x < s->width; x++) {
        for(int y = 0; y < s->height; y++) {
            Pixel * p = s->pixelAt(x,y);
            if(p->isRGB) {
                r = p->red;
                g = p->green;
                b = p->blue;
            } else {
                HSVtoRGB(&r,&g,&b,p->hue,p->saturation,p->value);
            }
            colorSpace(r*255,g*255,b*255);
        }
        resetColor();
        printf("\n");
    }
}
void forceRANDOM() {
    char * s;
    s = getenv("FRAND");
    if(s != 0) {
        int r = atoi(s);
        srand(r);
    }
}


void customRANDOM(){
    char * s;
    s = getenv("RAND");
    if(s == 0) {
        SEEDRANDOM;
        forceRANDOM();
    } else {
        int r = atoi(s);
        srand(r);
    }
}

void forceAlgorithm(int * alg) {
    char * s;
    s = getenv("FALG");
    if(s != 0) {
        *alg = atoi(s);
    }
} 

int getSize() {
    char * s;
    s = getenv("PIXELS");
    if(s != 0) {
        return atoi(s);
    }
    return 8;
}

int main(){
    int alg;
    int run = 0;
    int size = getSize();
    customRANDOM();
    while(true) {
        run++;
        clearScreen();
        printf("- Run %d -\n",run);
        forceRANDOM();
        Screen s(size,size);
        Algorithm * a;
        // Randomly select an algorithm
        alg = RANDOM() % 5;
        forceAlgorithm(&alg);
        switch(alg) {
            case 0: a = new HotPlate(); break;
            case 1: a = new GameOfLife(); break;
            case 2: a = new AStarTest(); break;
            case 3: a = new Snake(); break;
            case 4: a = new CornellBox(); break;

            default: a = new CornellBox(); break;
        }

        a->init(&s);
        do {
            putTermColor(&s);
            usleep(a->stepDelay() * 1000);
            //sleep(1);
        } while(a->step(&s));
        putTermColor(&s);
        sleep(1);
        delete a;
    }
}


