#pragma once

#include <variant>

#include "color.hpp"
#include "utils.hpp"

struct scatter_out {
  color attenuation;
  vec3 new_direction;
};
class Lambertian {
public:
  Lambertian(color albedo) : albedo(albedo) {}
  scatter_out scatter(const hit &h, rng &r) const {
    vec3 new_direction = random_unit_vector(r) + h.n;
    return {albedo, new_direction.normalize()};
  }

private:
  color albedo;
};

using material = std::variant<Lambertian>;
