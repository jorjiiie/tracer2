#pragma once

#include <cmath>

#include "common.hpp"
#include "debug.hpp"

struct vec3 {
  Float x, y, z;
  vec3() : x(0), y(0), z(0) {}
  vec3(Float x_, Float y_, Float z_) : x(x_), y(y_), z(z_) {}
  // the rest should be trivially generated

  Float operator[](int i) const {
    switch (i) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      [[unlikely]] return 0; // maybe throw
    }
  }
  Float &operator[](int i) {
    switch (i) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    }
    [[unlikely]] CERR_WHEN(
        LOG_WARNING, std::format("Index {} out of bounds on vec3 access", i));
    // insta crash because this should never happen
    exit(1);
  }

  vec3 operator+(const vec3 &v) const {
    vec3 r(*this);
    return r += v;
  }
  vec3 &operator+=(const vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  vec3 operator-(const vec3 &v) const {
    vec3 r(*this);
    return r -= v;
  }

  vec3 &operator-=(const vec3 &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  vec3 operator*(Float s) const {
    vec3 r(*this);
    return r *= s;
  }
  vec3 &operator*=(Float s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }
  vec3 operator/(Float s) const {
    vec3 r(*this);
    return r /= s;
  }
  vec3 &operator/=(Float s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
  }
  Float dot(const vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
  vec3 cross(const vec3 &v) const {
    return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }

  Float magsq() const { return dot(*this); }
  Float mag() const { return std::sqrt(magsq()); }
  vec3 &normalize() { return *this /= mag(); }

  std::string to_string() const {
    return std::format("[vec3: ({}, {}, {})]", x, y, z);
  }
};

inline Float dot(const vec3 &v1, const vec3 &v2) { return v1.dot(v2); }
inline vec3 cross(const vec3 &v1, const vec3 &v2) { return v1.cross(v2); }
