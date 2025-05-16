#pragma once

#include <variant>

#include "material.hpp"
#include "utils.hpp"
#include "vec3.hpp"

class Sphere {
public:
  Sphere(vec3 center_, Float radius_) : center(center_), radius(radius_) {}
  std::optional<hit> intersect(const vec3 &o, const vec3 &d) const {
    vec3 oc = o - center;
    Float a = dot(d, d);
    Float b = 2 * dot(oc, d);
    Float c = dot(oc, oc) - radius * radius;
    Float x1, x2;
    if (!quadratic(a, b, c, x1, x2)) {
      return std::nullopt;
    }
    // x2 is smaller
    std::optional<Float> t_opt = std::nullopt;
    if (x2 > 0) {
      t_opt = x2;
    } else if (x1 > 0) {
      t_opt = x1;
    }
    if (!t_opt) {
      return std::nullopt;
    }
    Float t = *t_opt;
    hit h;
    h.t = t;
    h.p = o + d * t;
    h.n = (h.p - center).normalize();
    h.w_i = d;
    return h;
  }

private:
  vec3 center;
  Float radius;
};

using shape = std::variant<Sphere>;

struct object {
  shape s;
  material m;
  color emission{};
};
