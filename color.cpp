#include <cstdio>
#include <cmath>
#include <cstdlib>

#include "color.h"

void colorSpace(int r, int g, int b) {
    int rep = 3;
    printf("\x1b[48;2;%d;%d;%dm",r,g,b);
    char * count = getenv("SWIDTH");
    if(count != 0) {
        rep = atoi(count);
    }

    for(int i = 0; i < rep; i++) {
        printf(" ");
    }
}
void resetColor() {
    printf("\x1b[0m");
}
void clearScreen() {
    printf("\033[2J");
}

// From https://www.cs.rit.edu/~ncs/color/t_convert.html
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
    int i;
    float f, p, q, t;
    if( s == 0 ) {
        // achromatic (grey)
        *r = *g = *b = v;
        return;
    }
    if(h >= 360.0) h -= 360.0;
    if(h < 0.0) h += 360.0;

    h /= 60;            // sector 0 to 5
    i = floor( h );
    f = h - i;          // factorial part of h
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );
    switch( i ) {
        case 0:
            *r = v; *g = t; *b = p;
            break;
        case 1:
            *r = q; *g = v; *b = p;
            break;
        case 2:
            *r = p; *g = v; *b = t;
            break;
        case 3:
            *r = p; *g = q; *b = v;
            break;
        case 4:
            *r = t; *g = p; *b = v;
            break;
        default:        // case 5:
            *r = v; *g = p; *b = q;
            break;
    }
}