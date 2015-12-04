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
    friend bool operator<(const Point & p1, const Point & p2) {
        if (p1.x < p2.x) return true;
        if (p1.y < p2.y) return true;
        return false;
    };
    friend bool operator==(const Point & p1, const Point & p2) {
        if (p1.x != p2.x) return false;
        if (p1.y != p2.y) return false;
        return true; 
    }
    Point() {
        x = 0;
        y = 0;
    }
    Point(int p1, int p2) {
        x = p1;
        y = p2;
    }
    Point(Point p, int offset_x, int offset_y)
#ifdef __clang__
    : Point()
#endif
{
        x = p.x + offset_x;
        y = p.y + offset_y;
    }
};

struct Screen {
    Screen(int,int);
    ~Screen();
    Pixel * pixels;
    int width;
    int height;
    void init();
    int totalPixels();
    Pixel * pixelAt(int, int);
};
