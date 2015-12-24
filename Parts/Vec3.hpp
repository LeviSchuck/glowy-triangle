#pragma once

#include <cmath>

// Used / copied from / derived from 
// http://www.kevinbeason.com/smallpt/

template<typename T>
struct Vec3 {
  T x, y, z;
  Vec3<T>(T x_=0, T y_=0, T z_=0){
    x=x_;
    y=y_;
    z=z_;
  }
  Vec3<T> operator+(const Vec3<T> &b) const {
    return Vec3<T>(x+b.x,y+b.y,z+b.z);
  }
  Vec3<T> operator-(const Vec3<T> &b) const {
    return Vec3<T>(x-b.x,y-b.y,z-b.z);
  }
  Vec3<T> operator*(T b) const {
    return Vec3<T>(x*b,y*b,z*b);
  }
  Vec3<T> operator/(T b) const {
    return Vec3<T>(x/b,y/b,z/b);
  }
  Vec3<T> mult(const Vec3<T> &b) const {
    return Vec3<T>(x*b.x,y*b.y,z*b.z);
  }
  Vec3<T>& norm(){
    return *this = *this * (1/sqrt(x*x+y*y+z*z));
  }
  T dot(const Vec3<T> &b) const {
    return x*b.x+y*b.y+z*b.z;
  }
  Vec3<T> operator%(Vec3<T>&b){
    return Vec3<T>(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
  }
};