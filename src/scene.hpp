#pragma once

#include <omp.h>

#include "camera.hpp"
#include "color.hpp"
#include "common.hpp"
#include "render.hpp"
#include "shape.hpp"

struct scene_options {
  Camera cam;
  std::vector<object> objects;
  color background;
};
struct render_options {
  int height;
  int width;
  int samples;
  int bounces;
  Float gamma;
  int threads = 1;
  int tile_size = 16;
  int rr_depth = 3;
};

class Scene {
public:
  Scene(const scene_options &so) : camera(so.cam), background(so.background) {
    objects.reserve(so.objects.size());
    for (const auto &obj : so.objects) {
      add_object(obj);
    }
  }

  void add_objects(const std::vector<object> &objs) {
    objects.insert(objects.end(), objs.begin(), objs.end());
  }
  void add_object(const object &obj) { objects.push_back(obj); }
  void add_object(object &&obj) { objects.emplace_back(obj); }

  std::optional<hit> intersect(const vec3 &o, const vec3 &d) const {
    std::optional<hit> hit_opt = std::nullopt;
    for (const auto &obj : objects) {
      auto hit_opt_ =
          std::visit([&](auto &&arg) { return arg.intersect(o, d); }, obj.s);
      if (!hit_opt_)
        continue;
      if (!hit_opt) {
        hit_opt = *hit_opt_;
        hit_opt->obj = &obj;
      } else {
        if (hit_opt_->t > hit_opt_->t) {
          hit_opt = *hit_opt_;
          hit_opt->obj = &obj;
        }
      }
    }
    return hit_opt;
  }

  image render(const render_options &ro) {
    camera.init(ro.height, ro.width);

    image result;
    result.data.resize(ro.height * ro.width);
    result.height = ro.height;
    result.width = ro.width;

    std::vector<rng> rngs;
    int n_threads = 1;
#ifdef _OPENMP
    omp_set_num_threads(ro.threads);
    n_threads = ro.threads;
#endif
    std::random_device rd{};
    for (int i = 0; i < n_threads; i++) {
      rngs.emplace_back(rng{rd()});
    }

#pragma omp parallel for collapse(2) schedule(dynamic)
    for (int r = 0; r < ro.height; r += ro.tile_size) {
      for (int c = 0; c < ro.width; c += ro.tile_size) {

        int rng_idx = 0;
#ifdef _OPENMP
        rng_idx = omp_get_thread_num();
#endif

        for (int rr = r; rr < std::min(r + ro.tile_size, ro.height); rr++) {
          for (int cc = c; cc < std::min(c + ro.tile_size, ro.width); cc++) {
            color pixel_color{};
            for (int s = 0; s < ro.samples; s++) {
              vec3 co = camera.get_origin();

              vec3 dir = camera.get_ray(rr, cc, rngs[rng_idx]);
              pixel_color += integrate(*this, ro, co, dir, rngs[rng_idx]);
            }
            // postprocess
            pixel_color /= ro.samples;

            for (int idx = 0; idx < 3; idx++)
              if (pixel_color[idx] > 0.0)
                pixel_color[idx] = std::pow(pixel_color[idx], 1.0 / ro.gamma);

            result.data[rr * ro.width + cc] = pixel_color;
          }
        }
      }
    }

    return result;
  }
  const std::vector<object> &get_objects() const { return objects; }
  const color background;

private:
  Camera camera;
  std::vector<object> objects;
};
