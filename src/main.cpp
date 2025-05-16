#include <fstream>
#include <iostream>

#include "color.hpp"
#include "debug.hpp"
#include "scene.hpp"
#include "utils.hpp"

int main() {
  Sphere s{vec3{0, 1, 0}, 0.5};
  Lambertian l{color{1, 0.2, 0.2}};
  Sphere s2{vec3{0.4, 0.8, -0.5}, 0.2};
  Lambertian l2{color{0.2, 0.8, 0.5}};
  Sphere s3{vec3{-0.4, 0.8, 0.5}, 0.2};
  Lambertian l3{color{0.2, 0.3, 0.9}};

  object o{s, l};
  object o2{s2, l2};
  object o3{s3, l3};

  camera_options co{.origin = vec3{0, -1, 0},
                    .direction = vec3{0, 1, 0},
                    .up = vec3{0, 0, 1},
                    .fov = 70};
  Camera c{co};
  scene_options so{
      .cam = c, .objects = {o, o2, o3}, .background = color{0.5, 0.7, 1}};
  Scene sc{so};
  render_options ro{.height = 2000,
                    .width = 3000,
                    .samples = 100,
                    .bounces = 7,
                    .gamma = 2.2,
                    .threads = 3,
                    .tile_size = 16};

  auto img = sc.render(ro);
  std::ofstream of("test2.ppm");
  write_ppm(of, img);
}
