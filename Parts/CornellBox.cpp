#include <random>

#include "Parts/CornellBox.hpp"

#include "color.h"
#include "defs.h"

const double sizecenter = 22;

CornellBox::CornellBox() {
  Ray c(Vec3<double>(50,52,295.6),Vec3<double>(0,-0.042612,-1).norm());
  setCamera(c);
  std::mt19937 gen(RANDOM());
  std::normal_distribution<> sized(sizecenter,5);
  std::normal_distribution<> posd(40,10);

  auto hsv2vec3 = [](float h, float s, float v) {
    float r,g,b;
    HSVtoRGB(&r,&g,&b,h,s,v);
    return Vec3<double>(r,g,b);
  };
  auto clamp = [](float v) {
    if(v < 0) return 0.0f;
    if(v > 1) return 1.0f;
    return v;
  };
  auto randColor = [&](){
    const float rate = 2;
    std::exponential_distribution<> d(rate+1);
    std::uniform_real_distribution<float> h(0,360);
    return hsv2vec3(
      h(gen),
      clamp((rate-d(gen))/rate),
      clamp((rate-d(gen))/rate)
    );
  };

  // Set objects
  auto makeWall = [&](Vec3<double> p, Vec3<double> c)
  {
    auto s = new Sphere(1e5);
    s->position = p;
    s->color = c;
    s->material_type = M_DIFFUSE;
    addShape(s);
  };

  auto makeLight = [&](Vec3<double> p, Vec3<double> c)
  {
    auto s = new Sphere(600);
    s->position = p;
    s->emission = c;
    s->material_type = M_DIFFUSE;
    addShape(s);
  };
  auto makeGlass = [&](double size, Vec3<double> p)
  {
    if(size == -1) {
      size = sized(gen);
      if(size < 1) size = 1;
      p.y += size;
      if(p.x - size < 0) p.x = size;
      if(p.z - size < 0) p.z = size;
      if(p.x + size > 81.6) p.x = 81.6 - size;
      if(p.z + size > 81.6) p.z = 81.6 - size;
    }
    auto s = new Sphere(size);
    s->position = p;
    s->color = Vec3<double>(1,1,1)*.999;
    s->material_type = M_REFRACTION;
    addShape(s);
  };
  auto makeMirror = [&](double size, Vec3<double> p)
  {
    if(size == -1) {
      size = sized(gen);
      if(size < 1) size = 1;
      p.y += size;
      if(p.x - size < 0) p.x = size;
      if(p.z - size < 0) p.z = size;
      if(p.x + size > 81.6) p.x = 81.6 - size;
      if(p.z + size > 81.6) p.z = 81.6 - size;
    }
    auto s = new Sphere(size);
    s->position = p;
    s->color = Vec3<double>(1,1,1)*.999;
    s->material_type = M_SPECULAR;
    addShape(s);
  };

  makeWall(Vec3<double>(1e5+1,40.8,81.6), randColor()); // right
  makeWall(Vec3<double>(-1e5+99,40.8,81.6), randColor());// left

  makeWall(Vec3<double>(50,40.8, 1e5), Vec3<double>(.75,.75,.75)); // back
  makeWall(Vec3<double>(50, 1e5, 81.6),Vec3<double>(.75,.75,.75)); // top
  makeWall(Vec3<double>(50,-1e5+81.6,81.6),Vec3<double>(.75,.75,.75)); // bottom

  
  
  makeGlass(-1, Vec3<double>(posd(gen)+5,posd(gen),posd(gen)/2+40));

  makeMirror(-1, Vec3<double>(posd(gen)-5,0,posd(gen)));
  
  makeLight(Vec3<double>(50,681.6-0.27,81.6),Vec3<double>(12,12,12));

  enumerateLights();
}

CornellBox::~CornellBox() {
}