#include "model.hxx"
#include <cmath>
#include <stdlib.h>
#include <math.h>

Model::Model(Game_config const& config)
        : config(config)
{
    blade_path_coarse = std::vector<ge211::Posn<int>>(config.blade_length,
            ge211::Posn<int>(-1, -1));
    blade_path_fine = std::vector<ge211::Posn<int>>(config.blade_length
            *config.blade_resolution, ge211::Posn<int>(-1, -1));
}

void
Model::on_frame(double dt)
{
    update_blade();
    if (!game_over) {
        if (abs(elapsed_time/5.0 - round(elapsed_time/5.0)) < 0.002) {
            swipe_combo = 0;
            generate_fruitsnbombs();
        }
        update_bomb(dt);
        add_combo_points();
        update_fruit(dt);
        pause_game();
        elapsed_time += dt;
        if (display_combo) {
            display_time += dt;
            if (display_time > 2.0) {
                display_combo = false;
            }
        }
    }
    else {
        display_combo = false;
        update_gamestate();
    }
}

void
Model::generate_fruitsnbombs(void)
{
    int num_fruits = round(elapsed_time/15) + (rand() % 3) - 1;
    if (num_fruits < 1) {
        num_fruits = 1;
    }
    int num_bombs = round(elapsed_time/30) + (rand() % 3) - 1;
    if (num_bombs < 0) {
        num_bombs = 0;
    }

    for (int i = 0; i < num_fruits; i++) {
        int x = rand() % config.scene_dims.width;
        int y = config.scene_dims.height+ (rand() % 400);
        int vx = rand() % 200;
        if (x > config.scene_dims.width/2) {
            vx = -vx;
        }
        int vy = -900 - (rand() % 200);
        Fruit new_fruit(Position(x,y), Velocity(vx, vy),type_generator(),
                        config);
        fruits.push_back(new_fruit);
    }
    for (int i = 0; i < num_bombs; i++) {
        int x = rand() % config.scene_dims.width;
        int y = config.scene_dims.height + (rand() % 400);
        int vx = rand() % 200;
        if (x > config.scene_dims.width/2) {
            vx = -vx;
        }
        int vy = -900 - (rand() % 200);
        Bomb new_bomb(Position(x,y), Velocity(vx, vy),0, config);
        bombs.push_back(new_bomb);
    }
}

void
Model::update_blade(void)
{
    if (mouse_pressed) {
        blade_path_coarse.insert(blade_path_coarse.begin(), mouse_position);
        blade_path_coarse.pop_back();
        interpolate();
    }
    else {
        blade_path_coarse = std::vector<ge211::Posn<int>>
                (config.blade_length,ge211::Posn<int>(-1,-1));
        blade_path_fine = std::vector<ge211::Posn<int>>
                (config.blade_length*config.blade_length, ge211::Posn<int>
                        (-1,-1));
    }
}

void
Model::update_bomb(double dt)
{
    for (Bomb& bomb : bombs) {
        if (bomb.is_active() && mouse_pressed) {
            if (!bomb.blade_entered && bomb.inside_fruit(mouse_position)) {
                bomb.blade_entered = true;
            } else if (bomb.blade_entered &&
                       !bomb.inside_fruit(mouse_position)) {
                bomb = bombs.back();
                bombs.pop_back();
                num_lives = 0;
                mouse_pressed = false;
                game_over = true;
            }
        }
        else {
            bomb.blade_entered = false;
        }
        if (!bomb.below_screen()) {
            bomb.advance_position(dt);
        }
        else {
            bomb = bombs.back();
            bombs.pop_back();
        }
    }
}

void
Model::update_fruit(double dt)
{
    if (!mouse_pressed) {
        swipe_combo = 0;
    }
    for (Fruit& fruit : fruits) {
        if (fruit.is_active() && mouse_pressed) {
            if (!fruit.blade_entered && fruit.inside_fruit(mouse_position)) {
                fruit.blade_entered = true;
            } else if (fruit.blade_entered &&
                       !fruit.inside_fruit(mouse_position)) {
                fruit.make_inactive();
                increase_score(1);
                swipe_combo += 1;
            }
        }
        else {
            fruit.blade_entered = false;
        }
        if (!fruit.below_screen()) {
            fruit.advance_position(dt);
        }
        else {
            if (fruit.is_active()) {
                decrease_lives();
                if (game_over) {
                    break;
                }
            }
            fruit = fruits.back();
            fruits.pop_back();
        }
    }
}

void
Model::update_gamestate(void)
{
    if (mouse_pressed) {
        if (!play_resume.blade_entered &&
        play_resume.inside_button(mouse_position)) {
            play_resume.blade_entered = true;
        } else if (play_resume.blade_entered &&
        !play_resume.inside_button(mouse_position)) {
            if (num_lives == 0) {
                restart_game();
            }
            else {
                game_over = false;
            }
            return;
        }
        if (!restart.blade_entered && restart.inside_button(mouse_position)) {
            restart.blade_entered = true;
        } else if (restart.blade_entered &&
                   !restart.inside_button(mouse_position)) {
            restart_game();
            return;
        }
    }
    else {
        play_resume.blade_entered = false;
        restart.blade_entered = false;
    }
}

void
Model::add_combo_points(void)
{
    if (!mouse_pressed && swipe_combo >= 3) {
        combo_points = pow(2, swipe_combo);
        increase_score(combo_points);
        last_mouse_position = mouse_position;
        display_combo = true;
        display_time = 0.0;
    }
}

void
Model::restart_game(void)
{
    fruits.clear();
    bombs.clear();
    score = 0;
    num_lives = 3;
    elapsed_time = 0.0;
    game_over = false;
}

void
Model::pause_game(void)
{
    if (mouse_pressed) {
        if (!pause.blade_entered && pause.inside_button(mouse_position)) {
            pause.blade_entered = true;
        } else if (pause.blade_entered &&
                   !pause.inside_button(mouse_position)) {
            mouse_pressed = false;
            pause.blade_entered = false;
            game_over = true;
        }
    }
    else {
        pause.blade_entered = false;
    }
}

void
Model::set_mouse_pressed(bool state)
{
    mouse_pressed = state;
}

void
Model::set_mouse_position(ge211::Posn<int> pos)
{
    mouse_position = pos;
}

void
Model::interpolate(void)
{
    if (blade_path_coarse[1].x >= 0) {
        double x_diff = blade_path_coarse[1].x - blade_path_coarse[0].x;
        double slope = (blade_path_coarse[1].y - blade_path_coarse[0].y)/
                x_diff;
        if (isnan(slope)) {
            return;
        }
        else if (slope == - INFINITY) {
            slope = -1000.0;
        }
        else if (slope == INFINITY) {
            slope = 1000.0;
        }
        double intercept = blade_path_coarse[0].y -
                slope*blade_path_coarse[0].x;
        double dx = x_diff/(config.blade_resolution-1);
        std::vector<ge211::Posn<int>> intrp_vec(config.blade_resolution,
                                                ge211::Posn<int>(-1, -1));
        for (int i = 0; i < config.blade_resolution; i++) {
            double x = blade_path_coarse[0].x + i*dx;
            int y = slope*x + intercept;
            intrp_vec[i] = ge211::Posn<int>((int) x, y);
        }
        blade_path_fine.insert(blade_path_fine.begin(), intrp_vec.begin(),
                               intrp_vec.end());
        for (int i = 0; i < config.blade_resolution; i++) {
            blade_path_fine.pop_back();
        }
    }
}

unsigned int
Model::get_score(void) const
{
    return score;
}

void
Model::increase_score(unsigned int added_points)
{
    int previous_score = score;
    score += added_points;
    regain_life(previous_score);
}

unsigned int
Model::get_numlives(void) const
{
    return num_lives;
}

int
Model::type_generator()
{
    std::vector<int> v{0,1,2};
    int random = rand() % v.size();
    int sel_elem = v[random];
    return sel_elem;
}

void
Model::regain_life(unsigned int prev_score)
{
    unsigned int nearest_hundred = ceil(prev_score/100.0)*100;
    if (prev_score < nearest_hundred && score >= nearest_hundred &&
        num_lives < 3) {
        num_lives += 1;
    }
}

void
Model::decrease_lives(void)
{
    if (num_lives > 0) {
        num_lives -= 1;
    }
    if (num_lives == 0) {
        game_over = true;
    }
}