#include "menu.hxx"
#include "game_config.hxx"

Menu::Menu(Position pos, Game_config const& config)
         : blade_entered(false),
           radius(config.button_radius),
           center(pos)
{ }

Menu::Menu(Position pos, int rad, Game_config const& config)
        : blade_entered(false),
          radius(rad),
          center(pos)
{ }

Position
Menu::top_left(void) const
{
    return Position(center.x-radius, center.y-radius);
}

bool
Menu::inside_button(ge211::Posn<int> posn) const
{
    return (posn.x - center.x)*(posn.x - center.x) + (posn.y - center.y)*
    (posn.y - center.y) < radius*radius;
}
