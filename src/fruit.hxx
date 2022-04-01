#pragma once

#include "game_config.hxx"
#include <ge211.hxx>
#include <iostream>

using Position = ge211::Posn<float>;
using Velocity = ge211::Dims<float>;

struct Fruit
{
public:
    Fruit(Game_config const& config = Game_config());
    Fruit(Position coord, Velocity vel, int type, Game_config const& config =
            Game_config());
    void advance_position(double dt);
    Position top_left(void) const;
    bool below_screen(void) const;
    bool inside_fruit(ge211::Posn<int> posn) const;
    void make_inactive(void);
    bool is_active(void) const;
    Fruit& operator=(const Fruit& other);
    bool blade_entered;
    int get_type(void);

private:
    Game_config const config;
    int radius;
    Position center;
    Velocity velocity;
    bool live;
    bool ascending;
    int type;
};
