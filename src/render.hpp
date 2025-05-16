#pragma once

#include "utils.hpp"

class Scene;
struct render_options;
// iteratively integrates
color integrate(const Scene &s, const render_options &ro, vec3 o, vec3 d,
                rng &r = rng::get());
