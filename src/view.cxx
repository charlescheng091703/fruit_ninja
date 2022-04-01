#include "view.hxx"
#include "model.hxx"
#include <vector>

static ge211::Color const blade_color {255, 255, 255};

View::View(Model const& m)
        : model(m),
          blade_sprite(m.config.blade_radius, blade_color),
          life_sprite("life.png"),
          lost_life_sprite("lost_life.png"),
          score_sprite("0", ge211::Font("go3v2.ttf", 25)),
          menu_button("play_game.png"),
          restart_button("restart_button.png"),
          point_icon("point_icon.png"),
          bomb_sprite("bomb.png"),
          pause_sprite("pause_button.png"),
          combo_sprite("Combo +0", ge211::Font("go3v2.ttf", 25)),
          watermelon("watermelon.svg"),
          watermelon_sliced("watermelon_sliced.png"),
          watermelon_anime({watermelon,watermelon_sliced}),
          kiwi("kiwi.svg"),
          kiwi_sliced("kiwi_sliced.png"),
          kiwi_anime({kiwi,kiwi_sliced}),
          lemon("lemon.svg"),
          lemon_sliced("lemon_sliced.png"),
          lemon_anime({lemon,lemon_sliced}),
          fruit_anime({watermelon_anime,kiwi_anime,lemon_anime}),
          background("background.png"),
          game_over("GAME OVER",ge211::Font("go3v2.ttf", 70)),
          title("title.png"),
          ninja("ninja.png")
{ }

void
View::draw(ge211::Sprite_set& sprites)
{
    sprites.add_sprite(background,ge211::Posn<int> {0,0});
    sprites.add_sprite(point_icon,ge211::Posn<int> {15,20},1);

    if (!model.game_over) {
        draw_bombs(sprites);
        draw_fruits(sprites);
        sprites.add_sprite(pause_sprite, model.pause.top_left().into<int>()
                .up_by(50),1);
    }
    else if (model.get_numlives() == 0) {
        sprites.add_sprite(game_over,model.play_resume.top_left()
                                          .into<int>().up_by
                                                  (200),1);
        draw_menu(sprites);
    }
    else {
        draw_menu(sprites);
    }
    draw_blade(sprites);
    draw_score(sprites);
    draw_lives(sprites);
    draw_combo(sprites);
}

ge211::Dims<int>
View::initial_window_dimensions() const
{
    return model.config.scene_dims;
}

void
View::draw_fruits(ge211::Sprite_set& sprites)
{
    for (Fruit fruit : model.fruits) {
        if (fruit.is_active()) {
            sprites.add_sprite(fruit_anime[fruit.get_type()].front(),
                               fruit.top_left().into<int>(),2);
        } else {
            sprites.add_sprite(fruit_anime[fruit.get_type()].back(),
                               fruit.top_left().into<int>(),2);
        }
    }
}

void
View::draw_bombs(ge211::Sprite_set& sprites)
{
    for (Bomb bomb : model.bombs) {
        sprites.add_sprite(bomb_sprite, bomb.top_left().into<int>(),2);
    }
}

void
View::draw_blade(ge211::Sprite_set& sprites)
{
    for (ge211::Posn<int> blade_section : model.blade_path_fine) {
        if (blade_section.x >= 0) {
            sprites.add_sprite(blade_sprite, blade_section,1);
        }
    }
}

void
View::draw_lives(ge211::Sprite_set& sprites)
{
    if (model.get_numlives() == 3) {
        sprites.add_sprite(life_sprite, ge211::geometry::Posn<int>(800, 15),1);
        sprites.add_sprite(life_sprite, ge211::geometry::Posn<int>(870, 25),1);
        sprites.add_sprite(life_sprite, ge211::geometry::Posn<int>(938, 35),1);
    }
    else if (model.get_numlives() == 2) {
        sprites.add_sprite(lost_life_sprite, ge211::geometry::Posn<int>(785, 6)
                ,1);
        sprites.add_sprite(life_sprite, ge211::geometry::Posn<int>(870, 25),1);
        sprites.add_sprite(life_sprite, ge211::geometry::Posn<int>(938,
                                                                        35),1);
    }
    else if (model.get_numlives() == 1) {
        sprites.add_sprite(lost_life_sprite, ge211::geometry::Posn<int>(785, 6)
                ,1);
        sprites.add_sprite(lost_life_sprite, ge211::geometry::Posn<int>(855,
                                                                        16),1);
        sprites.add_sprite(life_sprite, ge211::geometry::Posn<int>(938,
                                                                        35),1);
    }
    else {
        sprites.add_sprite(lost_life_sprite, ge211::geometry::Posn<int>(785,
                                                                        6),1);
        sprites.add_sprite(lost_life_sprite, ge211::geometry::Posn<int>(855,
                                                                        16),1);
        sprites.add_sprite(lost_life_sprite, ge211::geometry::Posn<int>(928,
                                                                        26),1);
    }
}

void
View::draw_score(ge211::Sprite_set& sprites)
{
    sprintf(score_text, "%d", model.get_score());
    score_sprite = ge211::Text_sprite(score_text, ge211::Font("go3v2.ttf",
                                                              70));
    sprites.add_sprite(score_sprite, ge211::geometry::Posn<int>(100, 10),1);
}

void
View::draw_combo(ge211::Sprite_set& sprites)
{
    if (model.display_combo) {
        sprintf(combo_text, "Combo +%d", model.combo_points);
        combo_sprite = ge211::Text_sprite(combo_text,
                                          ge211::Font("go3v2.ttf", 40));
        sprites.add_sprite(combo_sprite, model.last_mouse_position,1);
    }
}

void
View::draw_menu(ge211::Sprite_set& sprites)
{
    if (model.elapsed_time == 0 && !model.get_numlives() == 0) {
        sprites.add_sprite(title,model.play_resume.top_left()
                                      .into<int>().up_by
                                              (300).left_by(200),1);
        sprites.add_sprite(menu_button, model.play_resume.top_left()
                                             .into<int>().right_by(100), 1);
        sprites.add_sprite(ninja,ge211::geometry::Posn<int>(100, 300),1);
    }
    else if (model.get_numlives() == 0) {
        sprites.add_sprite(menu_button, model.play_resume.top_left()
                                             .into<int>().right_by(100), 1);
    }
    else {
        sprites.add_sprite(menu_button,
                           model.play_resume.top_left().into<int>(),
                           1);
        sprites.add_sprite(restart_button,
                           model.restart.top_left().into<int>().down_by(15),
                           1);
    }
}

