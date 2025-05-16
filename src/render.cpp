#include "render.hpp"
#include "scene.hpp"

color integrate(const Scene &s, const render_options &ro, vec3 o, vec3 d,
                rng &r) {
  color result{};
  color throughput{1, 1, 1};
  // CERR_OUT(std::format("integrate {} {}", o.to_string(), d.to_string()));

  for (int i = 0; i < ro.bounces; i++) {
    auto hit_opt = s.intersect(o, d);
    if (!hit_opt) {
      // empty!
      result += throughput * s.background;
      // CERR_OUT("intersected not");
      break;
    }
    // we have a hit!
    auto hit = *hit_opt;

    result += throughput * hit.obj->emission;

    auto material = hit.obj->m;
    scatter_out s =
        std::visit([&](auto &&m) { return m.scatter(hit, r); }, material);

    throughput *= s.attenuation;

    o = hit.p;
    d = s.new_direction;

    if (i > ro.rr_depth) {
      // this should be dependent on throughput
      // this seems expensive? invokes a sqrt...
      Float q = std::max(0.05, 1 - throughput.mag() * 0.33333);
      if (r.random_float() < q)
        break;
      throughput /= (1 - q);
    }
  }
  // CERR_OUT(std::format("hi! I got {}", result.to_string()));

  return result;
}
