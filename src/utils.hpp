#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
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

// not very thread safe!
struct rng {
  std::mt19937 m_rng;
  static rng &get() {
    static rng instance;
    return instance;
  }
};
// sample a random unit vector
inline vec3 random_unit_vector(rng &rand) {
  std::uniform_real_distribution<Float> dist(0.0, 1.0);
  Float u = dist(rand.m_rng);
  Float v = dist(rand.m_rng);
  Float theta = 2 * std::numbers::pi * u;
  Float phi = std::acos(2 * v - 1);

  return vec3{std::sin(phi) * std::cos(theta), std::sin(phi) * std::sin(theta),
              std::cos(phi)};
};
inline Float random_float(rng &rand, Float lo = 0.0, Float hi = 1.0) {
  std::uniform_real_distribution<Float> dist(lo, hi);
  return dist(rand.m_rng);
}

// assumes f in [0, 1]
// scales f to [lo, hi]
template <class Float, class T> T scale(Float f, T lo, T hi) {
  T range = hi - lo;

  f = std::clamp(f, (Float)0, (Float)1);
  return (T)(f * range);
}

template <class Stream> Stream &write_ppm(Stream &file, const image &im) {
  std::cerr << "what\n";
  file << "P3\n"
       << im.width << " " << im.height << "\n"
       << "255\n";

  // the way we do this it's upside down.

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
