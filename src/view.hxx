#pragma once

#include <ge211.hxx>

struct Model;

struct View
{
public:
    explicit View(Model const&);
    ge211::Dims<int> initial_window_dimensions() const;
    void draw(ge211::Sprite_set&);

    Model const& model;

private:
    void draw_fruits(ge211::Sprite_set& sprites);
    void draw_bombs(ge211::Sprite_set& sprites);
    void draw_blade(ge211::Sprite_set& sprites);
    void draw_score(ge211::Sprite_set& sprites);
    void draw_lives(ge211::Sprite_set& sprites);
    void draw_menu(ge211::Sprite_set& sprites);
    void draw_combo(ge211::Sprite_set& sprites);

    ge211::Circle_sprite const blade_sprite;
    ge211::Image_sprite const life_sprite;
    ge211::Image_sprite const lost_life_sprite;
    ge211::Text_sprite score_sprite;
    ge211::Image_sprite const menu_button;
    ge211::Image_sprite const restart_button;
    ge211::Image_sprite const point_icon;
    ge211::Image_sprite const bomb_sprite;
    ge211::Image_sprite const pause_sprite;
    ge211::Text_sprite combo_sprite;
    ge211::Image_sprite const watermelon;
    ge211::Image_sprite const watermelon_sliced;
    std::vector<ge211::Image_sprite> watermelon_anime;
    ge211::Image_sprite const kiwi;
    ge211::Image_sprite const kiwi_sliced;
    std::vector<ge211::Image_sprite> kiwi_anime;
    ge211::Image_sprite const lemon;
    ge211::Image_sprite const lemon_sliced;
    std::vector<ge211::Image_sprite> lemon_anime;
    std::vector<std::vector<ge211::Image_sprite>> fruit_anime;
    ge211::Image_sprite const background;
    ge211::Text_sprite game_over;
    ge211::Image_sprite const title;
    ge211::Image_sprite const ninja;

    char score_text[100];
    char combo_text[100];
};
