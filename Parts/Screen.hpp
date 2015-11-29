#pragma once

struct Pixel {
    float hue;
    float saturation;
    float value;
    bool masked;
};

struct Point {
    int x;
    int y;
};

struct Screen {
public:
    Screen(int,int);
    ~Screen();
    Pixel * pixels;
    int width;
    int height;
    void init();
    int totalPixels();
    Pixel * pixelAt(int, int);
};