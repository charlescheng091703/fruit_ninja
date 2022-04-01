#include "game_config.hxx"

Game_config::Game_config()
        : scene_dims {1024, 768},
          fruit_radius(50),
          blade_radius(5),
          blade_length(8),
          blade_resolution(15),
          grav_const(9.8),
          button_radius(70)
{ }
