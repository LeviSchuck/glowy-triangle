#pragma once

struct Pixel {
    float hue;
    float saturation;
    float value;
    float red;
    float green;
    float blue;
    bool masked;
    bool isRGB;
    Pixel() {
        clear();
    }
    Pixel(float h, float s, float v) {
        hue = h;
        saturation = s;
        value = v;
        masked = false;
        isRGB = false;
    }
    Pixel(int r, int g, int b) {
        red = r / 255;
        green = g / 255;
        blue = b / 255;
        isRGB = true;
        masked = false;
    }
    Pixel(const Pixel & other) {
        hue = other.hue;
        saturation = other.saturation;
        value = other.value;
        red = other.red;
        green = other.green;
        blue = other.blue;
        masked = other.masked;
        isRGB = other.isRGB;
    }
    Pixel & operator=(const Pixel & other) {
        hue = other.hue;
        saturation = other.saturation;
        value = other.value;
        red = other.red;
        green = other.green;
        blue = other.blue;
        masked = other.masked;
        isRGB = other.isRGB;
        return *this;
    }
    void clear() {
        hue = saturation = value = red = green = blue = 0;
        masked = false;
        isRGB = false;
    }
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
