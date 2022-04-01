#include "bomb.hxx"
#include "game_config.hxx"

Bomb::Bomb(Game_config const& config)
        : Fruit(config)
{ }

Bomb::Bomb(Position coord, Velocity vel, int kind, Game_config const& config)
        : Fruit(coord, vel, kind, config)
{ }