#include <random>
#include <numeric>
#include <iostream>

#include "Parts/SmallPT.hpp"

constexpr double pi() { return 3.141592653589793; }

const double piFrac = 1 / pi();

// I don't know what this is.
inline Vec3<double> mabs(Vec3<double> v) {
  if(fabs(v.x) > 0.1) {
    return (Vec3<double>(0,1,0) % v).norm();
  } else {
    return (Vec3<double>(1,0,0) % v).norm();
  }
}

double r48() {
  static std::default_random_engine br;
  static std::minstd_rand gen(br());
  static std::uniform_real_distribution<double> distr(0.0,1.0);
  return distr(gen);
}


Maybe<double> Box::intersect(const Ray & r) const {
  return nothing<double>();
}
Maybe<double> Sphere::intersect(const Ray & r) const {
  // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
  Vec3<double> op = position - r.origin;
  double t1,t2, eps=1e-4;
  double b = op.dot(r.direction);
  double det = b * b - op.dot(op) + rad * rad;

  if(det >= 0) {
    det = sqrt(det);
    t1 = b - det;
    t2 = b + det;
    if(t1 > eps) {
      return just(t1);
    } if(t2 > eps) {
      return just(t2);
    }
  }
  return nothing<double>();
}

Vec3<double> Sphere::asLight(
  Vec3<double> x,
  Vec3<double> nl,
  Vec3<double> c,
  std::function<bool(const Ray &)> inter
  ) {
  auto sw = position - x;
  auto su = mabs(sw);
  auto sv = sw % su;

  auto p = x - position;

  double cos_a_max = sqrt(1 - rad * rad  / p.dot(p));
  double eps1 = r48(), eps2 = r48();
  double cos_a = 1 - eps1 + eps1 * cos_a_max;
  double sin_a = sqrt(1 - cos_a * cos_a);
  double phi = 2 * pi() * eps2;

  Vec3<double> l = su * cos(phi) * sin_a + sv * sin(phi) * sin_a + sw * cos_a;
  l.norm();
  if (inter(Ray(x,l))) {
    double omega = 2 * pi() * cos_a_max;
    double fact = l.dot(nl) * omega;
    if(fact < 0) return Vec3<double>();
    return c.mult(emission * fact) * piFrac;
  } else {
    return Vec3<double>();
  }
}

SmallPT::SmallPT() : camera (Ray(Vec3<double>(),Vec3<double>(1))) {
  nextKey = 0;
  samples = 0;
  fov = 0.5135;
  sampleEps = 1e-1;

}

SmallPT::~SmallPT(){
  clearScene();
}

void SmallPT::init(Screen * sc) {
  width = sc->width;
  height = sc->height;

  for(int x = 0; x < width; x++) {
    for(int y = 0; y < height; y++) {
        sc->pixelAt(x,y)->clear();
        sc->pixelAt(x,y)->isRGB = true;
    }
  }
  // 8 : 0.02
  // 10 : 0.05
  // 16: 0.16
  // 20: 0.26
  // 30: 1.25
  // 40: 3.2
  sampleEps = 3.498e-5 * pow((width + height) / 2, 3.06627265);
}

inline double clamp(double x) { return x<0 ? 0 : x>1 ? 1 : x; }


bool SmallPT::step(Screen * sc) {
  bool r = stepSample();
  for(int x = 0; x < width; x++) {
    for(int y = 0; y < height; y++) {
      // Upside down
      auto sample = sampled[Point(width-x-1,height-y-1)] / samples;
      

      Pixel * p = sc->pixelAt(x,y);
      p->red = clamp(sample.x);
      p->green = clamp(sample.y);
      p->blue = clamp(sample.z);
    }
  }
  return r;
}

SceneKey SmallPT::addShape(Graphics3DShape * shape) {
  SceneKey k = nextKey++;
  scene[k] = shape;
  return k;
}

void SmallPT::clearScene() {
  for(auto kv : scene) {
    if(kv.second) delete kv.second;
  }
  scene.clear();
  lights.clear();
  sampled.clear();
  samples = 0;
}

void SmallPT::enumerateLights(){
  lights.clear();
  for(auto kv : scene) {
    auto ptr = kv.second;
    if(ptr->emission.x > 0 || ptr->emission.y > 0 || ptr->emission.z > 0) {
      lights.insert(kv.first);
    }
  }
}

Maybe<RayResult<SceneKey>> SmallPT::intersect(const Ray & r, bool diffuse) {
  auto res = nothing<SceneKey>();
  double dist = 1e20;
  for(auto kv : scene) {
    auto ptr = kv.second;
    // Only diffuse materials can look at lights directly
    if(!diffuse && ptr->material_type == M_NONE) continue;
    // Test intersection
    ptr->intersect(r).on([&](double d){
      // Find closest
      if(d < dist) {
        dist = d;
        res = just(kv.first);
      }
    },[](){});
  }
  // Prepare result
  if(dist >= 1e20) {
    return nothing<RayResult<SceneKey>>();
  }
  return res.on([&](SceneKey k){
    RayResult<SceneKey> r;
    r.dist = dist;
    r.ref = k;
    return just(r);
  },[](){
    return nothing<RayResult<SceneKey>>();
  });
}

inline Vec3<double> reflection(const Ray & ray, Vec3<double> normal) {
  return ray.direction - (normal * (2 * normal.dot(ray.direction)));
}

Vec3<double> SmallPT::radiance(const Ray & ray, int depth, bool E) {
  if(depth > 15) {
    return Vec3<double>();
  }

  return intersect(ray).on([&](RayResult<SceneKey> res) {
    auto x = ray.origin + (ray.direction * res.dist);
    auto ptr = scene.at(res.ref);
    auto normal = (x - ptr->position).norm();
    Vec3<double> nl;
    if (normal.dot(ray.direction) < 0)
      nl = normal; 
    else
      nl = normal * -1;

    auto color = ptr->color;

    {
      double p;
      // Find max color channel
      if (color.x > color.y && color.x > color.z) {
        p = color.x;
      } else if (color.y > color.z) {
        p = color.y;
      } else {
        p = color.z;
      }

      // Handle Emission case
      if (p <= 0 || depth > 5) {
        if(r48() < p) color = color * (1/p);
        else if (E) return ptr->emission;
        else return Vec3<double>();
      }
    }

    // Handle the different materials.
    auto m = ptr->material_type;
    if (m == M_DIFFUSE) {
      double r1  = 2 * pi() * r48();
      double r2  =            r48();
      double r2s = sqrt(r2);

      auto u = mabs(nl);
      auto v = nl % u;

      // next direction
      auto d =
        ( u * cos(r1) * r2s
        + v * sin(r1) * r2s
        + nl * sqrt(1-r2)
        ).norm();

      // Lights
      Vec3<double> e;
      if(E) {
        e = ptr->emission;
      }
      
      
      for (auto k : lights) {
        auto ptr = scene.at(k);
        // Make sure not shadowed.
        e = e + ptr->asLight(x, nl, ptr->color, [&](const Ray & r) {
          return intersect(r).on([&](RayResult<SceneKey> res){
            return res.ref == k;
          }, [](){
            return false;
          });
        });
      }

      return e + color.mult(radiance(Ray(x,d),depth+1)); //,false
    } else if (m == M_SPECULAR) {
      auto reflect = reflection(ray,normal);
      return ptr->emission + color.mult(radiance(Ray(x,reflect),depth+1));
    } else if (m == M_REFRACTION) {
      Ray reflRay(x, reflection(ray,normal));
      bool into = normal.dot(nl) > 0;

      double nc = 1, nt = 1.5;
      double ddn = ray.direction.dot(nl);
      double nnt, cos2t;
      if(into) {
        nnt = nc / nt;
      } else {
        nnt = nt / nc;
      }
      cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
      // Internal reflection?
      if(cos2t < 0) {
        return ptr->emission + color.mult(radiance(reflRay, depth+1));
      }

      Vec3<double> tdir;
      {
        double tempdir;
        if(into) {
          tempdir = 1;
        } else {
          tempdir = -1;
        }
        auto temp1 = ray.direction * nnt;
        auto temp2 = normal * (tempdir * (ddn * nnt + sqrt(cos2t)));
        tdir = (temp1 - temp2).norm();
      }
      double a = nt - nc;
      double b = nt + nc;
      double R0 = a * a / (b * b);
      double c;
      if(into) {
        c = 1 + ddn;
      } else {
        c = 1 - tdir.dot(normal);
      }
      double Re = R0 + (1 - R0) * c * c * c * c * c;
      double Tr = 1 - Re;
      double P = 0.25 + 0.5*Re;
      double TP = Tr / (1 - P);
      double RP = Re/P;

      // Russian Roulette Refraction
      Vec3<double> rrr;
      if(depth < 2) {
        rrr = radiance(reflRay    ,depth+1) * Re
          +   radiance(Ray(x,tdir),depth+1) * Tr;
      } else if(r48() < P) {
        rrr = radiance(reflRay    ,depth+1) * RP;
      } else {
        rrr = radiance(Ray(x,tdir),depth+1) * TP;
      }

      return ptr->emission + color.mult(rrr);
    }

    

    return Vec3<double>();
  },[](){
    return Vec3<double>();
  });
}

void SmallPT::setCamera(Ray & r) {
  camera = r;
}

const int perpix = 1;

bool SmallPT::stepSample() {
  samples++;

  // Slight deviation, was sideways.
  Vec3<double> cx(0,width * fov / height,0);
  Vec3<double> cy = (cx % camera.direction).norm() * fov;

  double difference = 0;


  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      Vec3<double> r;
      for(int i = 0; i < perpix; i++) {
        double r1 = 2 * r48();
        double dx;
        if(r1 < 1) {
          dx = sqrt(r1) - 1;
        } else {
          dx = 1 - sqrt(2 - r1);
        }
        double r2 = 2 * r48();
        double dy;
        if(r2 < 1) {
          dy = sqrt(r2) - 1;
        } else {
          dy = 1 - sqrt(2 - r2);
        }
        Vec3<double> d 
              = cx * ( ( (1 + dx) / 4 + x) / width - 0.5)
              + cy * ( ( (1 + dy) / 4 + y) / height - 0.5)
              + camera.direction;
        r = r + radiance(Ray(camera.origin + d * 140, d.norm()),0);
      }
      r = r / perpix;
      
      Vec3<double> oldSample = sampled[Point(x,y)];
      Vec3<double> newSample = oldSample + r;
      sampled[Point(x,y)] = newSample;

      Vec3<double> diff = (newSample / samples);
      if(samples > 1) {
        diff = diff - (oldSample / (samples-1));
      }
      difference += fabs(diff.x);
      difference += fabs(diff.y);
      difference += fabs(diff.z);
    }
  }
  //std::cout << "Epsilon: " << difference << std::endl;
  return difference > sampleEps;
}