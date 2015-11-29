
#include "Parts/Screen.hpp"

Screen::Screen(int w, int h) {
    width = w;
    height = h;
    pixels = new Pixel[w*h];
    init();
}

Screen::~Screen() {
    delete[] pixels;
    pixels = 0;
}

int Screen::totalPixels(){
    return width * height;
}
Pixel * Screen::pixelAt(int x, int y) {
    if(x<0 || y<0 || x>=width || y>=width) return 0;
    return pixels + (y*width + x);
}

void Screen::init() {
    int total = totalPixels();
    for(int i = 0; i < total; i++) {
        pixels[i].hue = 0.0;
        pixels[i].saturation = 0.0;
        pixels[i].value = 0.0;
        pixels[i].masked = false;
    }
}