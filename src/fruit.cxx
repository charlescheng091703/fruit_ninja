#include "fruit.hxx"
#include "game_config.hxx"

Fruit::Fruit(Game_config const& config)
        : blade_entered(false),
          radius(config.fruit_radius),
          center(Position(0, 0)),
          velocity(Velocity(0, 0)),
          live(true),
          ascending(true),
          type(0)
{ }

Fruit::Fruit(Position coord, Velocity vel,int kind, Game_config const& config)
        : blade_entered(false),
          radius(config.fruit_radius),
          center(coord),
          velocity(vel),
          live(true),
          ascending(true),
          type(kind)
{ }

Fruit&
Fruit::operator=(const Fruit& other) {
    this->blade_entered = other.blade_entered;
    this->radius = other.radius;
    this->center = other.center;
    this->velocity = other.velocity;
    this->live = other.live;
    this->ascending = other.ascending;
    this->type = other.type;
    return *this;
}

void
Fruit::advance_position(double dt)
{
    center.x += dt*velocity.width;
    velocity.height += config.grav_const;
    float prev_y = center.y;
    center.y += dt*velocity.height;
    if (prev_y < center.y) {
        ascending = false;
    }
}

Position
Fruit::top_left(void) const
{
    return Position(center.x-radius, center.y-radius);
}

bool
Fruit::below_screen() const
{
    if (ascending) {
        return false;
    }
    return center.y-radius > config.scene_dims.height;
}

bool
Fruit::inside_fruit(ge211::Posn<int> posn) const
{
    return (posn.x - center.x)*(posn.x - center.x) + (posn.y - center.y)*
    (posn.y - center.y) < radius*radius;
}

void
Fruit::make_inactive(void)
{
    live = false;
}

bool
Fruit::is_active(void) const
{
    return live;
}

int
Fruit::get_type(void)
{
   return type;
}