#pragma once

#include <ge211.hxx>

struct Game_config
{
    Game_config();

    // The dimensions of the whole window:
    ge211::Dims<int> scene_dims;

    // The radius of the fruit:
    int fruit_radius;
    // The radius of the blade:
    int blade_radius;
    // The length of the blade:
    int blade_length;
    // Number of points interpolated between recorded mouse positions
    int blade_resolution;
    // gravitational constant
    float grav_const;
    // radius of the play, pause, restart buttons
    int button_radius;
};

