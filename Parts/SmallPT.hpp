#pragma once

// Used / copied from / derived from 
// http://www.kevinbeason.com/smallpt/

#include <map>
#include <set>
#include <memory>
#include <utility>
#include <functional>

#include "Parts/Algorithm.hpp"
#include "Parts/Vec3.hpp"
#include "Lib/Maybe.hpp"


// Material types
enum Refl_t {
  M_DIFFUSE,
  M_SPECULAR,
  M_REFRACTION,
  M_NONE
};

struct Material {
  Vec3<double> emission, color;
  Refl_t material_type;
};
struct Transformable {
  Vec3<double> position;
};

struct Ray {
  Vec3<double> origin, direction;
  Ray(Vec3<double> o, Vec3<double> d) : origin(o), direction(d) {
  }
};

template <typename R>
struct RayResult {
  double dist;
  R ref;
};

class Intersectable {
public:
  virtual Maybe<double> intersect(const Ray & r) const = 0; 
};


class Graphics3DShape : public Intersectable, public Material, public Transformable {
public:
  Graphics3DShape() {}
  virtual ~Graphics3DShape() {}
  virtual Vec3<double> asLight(Vec3<double> x, Vec3<double> nl, Vec3<double> c, std::function<bool(const Ray &)>) {
    return Vec3<double>();
  }
};

class Sphere : public Graphics3DShape {
public:
  double rad;
  Sphere(double rad_): rad(rad_) {
  }
  ~Sphere(){}
  virtual Maybe<double> intersect(const Ray & r) const;
  virtual Vec3<double> asLight(Vec3<double>, Vec3<double>, Vec3<double>, std::function<bool(const Ray &)>);
};

class Box : public Graphics3DShape {
public:
  double extents_x, extents_y, extents_z;
  Box(double x, double y, double z): extents_x(x), extents_y(y), extents_z(z) {
  }
  ~Box(){}
  virtual Maybe<double> intersect(const Ray & r) const;
};


typedef int SceneKey;

class SmallPT : public Algorithm {
public:
  SmallPT();
  virtual void init(Screen*);
  virtual bool step(Screen*);
  virtual ~SmallPT();
  virtual int stepDelay() {
    return 1;
  }
  SceneKey addShape(Graphics3DShape *);
  void clearScene();
  void enumerateLights();
  void setCamera(Ray &);
  bool stepSample();

protected:
  Maybe<RayResult<SceneKey>> intersect(const Ray & r, bool diffuse = false);
  Vec3<double> radiance(const Ray &r, int depth, bool E = true);
  std::map<SceneKey,Graphics3DShape *> scene;
  std::set<SceneKey> lights;
  int width, height;
  std::map<Point,Vec3<double>> sampled;
  Ray camera;
  double fov;
  double sampleEps;
private:
  SceneKey nextKey;
  int samples;
};

