#pragma once

#include "common.hpp"
#include "vec3.hpp"

struct color : public vec3 {
  using vec3::vec3;
  Float &r() { return this->x; }
  Float r() const { return this->x; }
  Float &g() { return this->y; }
  Float g() const { return this->y; }
  Float &b() { return this->z; }
  Float b() const { return this->z; }

  // element-wise multiplication
  color &operator*=(const color &o) {
    this->r() *= o.r();
    this->g() *= o.g();
    this->b() *= o.b();
    return *this;
  }
  color operator*(const color &o) {
    color t(*this);
    return t *= o;
  }
};
