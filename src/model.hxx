#pragma once

#include "fruit.hxx"
#include "bomb.hxx"
#include "menu.hxx"
#include "game_config.hxx"
#include <vector>

struct Model
{
public:
    bool game_over = true;

    explicit Model(Game_config const& config = Game_config());
    void on_frame(double dt);
    void set_mouse_pressed(bool state);
    void set_mouse_position(ge211::Posn<int> pos);
    unsigned int get_score(void) const;
    unsigned int get_numlives(void) const;
    int type_generator(void);

    Game_config const config;
    std::vector<Fruit> fruits;
    std::vector<Bomb> bombs;
    std::vector<ge211::Posn<int>> blade_path_coarse;
    std::vector<ge211::Posn<int>> blade_path_fine;
    Menu play_resume = Menu(Position(417, 550), config);
    Menu restart = Menu(Position(607, 550), config);
    Menu pause = Menu(Position(50, 720), 25, config);
    bool display_combo = false;
    ge211::geometry::Posn<int> last_mouse_position = ge211::Posn<int>(-1, -1);
    unsigned int combo_points = 0;
    double elapsed_time = 0.0;
    void increase_score(unsigned int added_points);
    unsigned int
    get_swipe_combo(void) const {
        return swipe_combo;
    }

private:
    void generate_fruitsnbombs(void);
    void update_blade(void);
    void update_bomb(double dt);
    void update_fruit(double dt);
    void interpolate();
    void regain_life(unsigned int prev_score);
    void decrease_lives();
    void update_gamestate(void);
    void restart_game(void);
    void add_combo_points(void);
    void pause_game(void);

    unsigned int swipe_combo = 0;
    bool mouse_pressed = false;
    unsigned int score = 0;
    unsigned int num_lives = 3;
    ge211::Posn<int> mouse_position = ge211::Posn<int>(-1, -1);
    double display_time = 0.0;
};

