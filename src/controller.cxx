#include "controller.hxx"
#include "model.hxx"

Controller::Controller(Model& model)
        : model(model),
          view(model)
{ }

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code('q')) {
        quit();
    }
}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    model.set_mouse_pressed(false);
}

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int>)
{
    model.set_mouse_pressed(true);
}

void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    model.set_mouse_position(position);
}

void
Controller::on_frame(double dt)
{
    model.on_frame(dt);
}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return view.initial_window_dimensions();
}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view.draw(sprites);
}
