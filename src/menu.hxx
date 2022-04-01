#pragma once

#include "game_config.hxx"
#include <ge211.hxx>
#include <iostream>

using Position = ge211::Posn<float>;
using Velocity = ge211::Dims<float>;

struct Menu
{
public:
    Menu(Position pos, Game_config const& config = Game_config());
    Menu(Position pos, int rad, Game_config const& config = Game_config());
    Position top_left(void) const;
    bool inside_button(ge211::Posn<int> posn) const;

    bool blade_entered;

private:
    Game_config const config;
    int radius;
    Position center;
};
