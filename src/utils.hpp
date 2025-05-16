#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numbers>
#include <random>
#include <vector>

#include "color.hpp"
#include "vec3.hpp"

struct object;

struct hit {
  Float t;
  vec3 p, n, w_i;
  const object *obj;
};

struct image {
  int width, height;
  // row major: (r, c) = row * width + c
  std::vector<color> data;
};

class rng_std {
public:
  rng_std() : m_rng(0) {}
  rng_std(uint64_t seed) : m_rng(seed) {}
  Float random_float(Float lo = 0.0, Float hi = 1.0) {
    std::uniform_real_distribution<Float> dist(lo, hi);
    return dist(m_rng);
  }

  static rng_std &get() {
    static rng_std instance;
    return instance;
  }

private:
  std::mt19937 m_rng;
};
using rng = rng_std;

inline vec3 random_unit_vector(rng &r) {
  Float u = r.random_float();
  Float v = r.random_float();
  Float theta = 2 * std::numbers::pi * u;
  Float phi = std::acos(2 * v - 1);

  return vec3{std::sin(phi) * std::cos(theta), std::sin(phi) * std::sin(theta),
              std::cos(phi)};
}

// scales f to [lo, hi], on a scale from [0, 1]
template <class Float, class T> T scale(Float f, T lo, T hi) {
  T range = hi - lo;

  f = std::clamp(f, (Float)0, (Float)1);
  return (T)(f * range);
}

template <class Stream> Stream &write_ppm(Stream &file, const image &im) {
  file << "P3\n"
       << im.width << " " << im.height << "\n"
       << "255\n";

  // the way we do this it's upside down since (0,0) is the bottom left
  // PPM goes top down
  for (int row = im.height - 1; row >= 0; row--) {
    for (int c = 0; c < im.width; c++) {
      const color &pix = im.data[row * im.width + c];
      int r = scale(pix.x, 0, 255);
      int g = scale(pix.y, 0, 255);
      int b = scale(pix.z, 0, 255);

      file << r << " " << g << " " << b << "\n";
    }
  }
  return file;
};

// this is not optimized but issok
[[gnu::hot]] inline bool quadratic(Float a, Float b, Float c, Float &x1,
                                   Float &x2) {
  // first use standard formula
  Float d = b * b - 4 * a * c;
  if (d < 0) {
    return false;
  }
  if (d == 0) {
    x1 = x2 = -b / (2 * a);
    return true;
  }
  x1 = (-b + std::sqrt(d)) / (2 * a);
  x2 = (-b - std::sqrt(d)) / (2 * a);
  return true;
}
