#pragma once

#include "common.hpp"
#include "utils.hpp"
#include "vec3.hpp"

struct camera_options {
  vec3 origin, direction, up;
  double fov;
};
class Camera {
public:
  Camera(camera_options co)
      : origin(co.origin), direction(co.direction), up(co.up), fov(co.fov) {
    right = cross(direction, up).normalize();
    up.normalize();
    direction.normalize();
  }
  void init(int height, int width) {
    double w = 2 * std::atan(fov / 2.0);
    lp = w / width;
    bl = origin + direction - (right * lp * width / 2.0) -
         (up * lp * height / 2.0);
  }

  vec3 get_ray(int row, int column, rng &r) const {
    return (bl + (right * lp * (column + r.random_float(-0.5, 0.5))) +
            (up * lp * (row + r.random_float(-0.5, 0.5))) - origin)
        .normalize();
  }
  vec3 get_origin() const { return origin; }

private:
  vec3 origin, direction, up, right, bl;
  double fov, lp;
};
