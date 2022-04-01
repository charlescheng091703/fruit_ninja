#pragma once

#include "game_config.hxx"
#include <ge211.hxx>
#include <iostream>
#include "fruit.hxx"

struct Bomb: public Fruit
{
public:
    Bomb(Game_config const& config = Game_config());
    Bomb(Position coord, Velocity vel,int kind, Game_config const& config =
            Game_config());
};