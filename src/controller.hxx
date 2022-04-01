#pragma once

#include "view.hxx"
#include <ge211.hxx>

struct Controller : ge211::Abstract_game
{
    explicit Controller(Model&);
    void on_key(ge211::Key) override;
    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_mouse_down(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_mouse_move(ge211::Posn<int>) override;
    void on_frame(double dt) override;
    ge211::Dims<int> initial_window_dimensions() const override;
    void draw(ge211::Sprite_set&) override;

    Model& model;
    View view;
};
