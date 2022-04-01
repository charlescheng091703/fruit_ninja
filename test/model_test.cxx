#include "model.hxx"
#include <catch.hxx>

Game_config const config;

// Slicing a bomb should end the game
TEST_CASE("BOMB_SLICE")
{
    Model model(config);
    model.game_over = false;
    Bomb new_bomb(Position(100,100), Velocity(0,0),0, config);
    model.bombs.push_back(new_bomb);
    CHECK_FALSE(model.game_over);
    CHECK(new_bomb.is_active());
    model.set_mouse_pressed(true);
    model.set_mouse_position(Position(20,100).into<int>());
    model.on_frame(0.0000001);
    model.set_mouse_position(Position(100,100).into<int>());
    model.on_frame(0.0000001);
    CHECK(model.bombs[0].blade_entered);
    model.set_mouse_position(Position(200,100).into<int>());
    model.on_frame(0.0000001);
    CHECK(model.game_over);
}

// If a fruit falls below the bottom of the screen then a life should be lost
TEST_CASE("LOST_LIFE")
{
    Model model(config);
    model.game_over = false;
    Fruit new_fruit(Position(100,100), Velocity(0,0),0, config);
    model.fruits.push_back(new_fruit);
    CHECK(model.fruits[0].is_active());
    CHECK(model.get_numlives() == 3);
    model.on_frame(100);
    CHECK(model.fruits[0].below_screen());
    model.on_frame(20);
    CHECK(model.get_numlives() == 2);
}

// If three fruit fall below the bottom of the screen then the game should end
TEST_CASE("LOST_THREE_LIVES")
{
    Model model(config);
    model.game_over = false;
    Fruit fruit1(Position(100,100), Velocity(0,0),0, config);
    Fruit fruit2(Position(200,100), Velocity(0,0),0, config);
    Fruit fruit3(Position(300,100), Velocity(0,0),0, config);
    model.fruits.push_back(fruit1);
    model.fruits.push_back(fruit2);
    model.fruits.push_back(fruit3);
    CHECK(model.fruits[0].is_active());
    CHECK(model.fruits[1].is_active());
    CHECK(model.fruits[2].is_active());
    CHECK(model.get_numlives() == 3);
    model.on_frame(100);
    CHECK(model.fruits[0].below_screen());
    CHECK(model.fruits[1].below_screen());
    CHECK(model.fruits[2].below_screen());
    model.on_frame(20);
    CHECK(model.get_numlives() == 0);
    CHECK(model.game_over);
}

// If a player has three lives and the points reach a multiple of 100 then no
// lives should be added
TEST_CASE("MAX_LIVES")
{
    Model model(config);
    model.game_over = false;
    CHECK(model.get_numlives() == 3);
    CHECK(model.get_score() == 0);
    model.increase_score(150);
    CHECK(model.get_score() == 150);
    CHECK(model.get_numlives() == 3);
}

// If a life has been lost and the points reach a multiple of 100 then one
// life is added
TEST_CASE("REGAIN_HEALTH")
{
    Model model(config);
    model.game_over = false;
    Fruit new_fruit(Position(100,100), Velocity(0,0),0, config);
    model.fruits.push_back(new_fruit);
    CHECK(model.fruits[0].is_active());
    model.set_mouse_pressed(true);
    CHECK(model.get_numlives() == 3);
    model.on_frame(100);
    CHECK(model.get_score() == 0);
    CHECK(model.fruits[0].below_screen());
    model.on_frame(20);
    CHECK(model.get_numlives() == 2);
    model.increase_score(75);
    CHECK(model.get_score() == 75);
    model.on_frame(100);
    model.increase_score(35);
    CHECK(model.get_score() == 110);
    model.set_mouse_pressed(false);
    model.on_frame(100);
    CHECK(model.get_numlives() == 3);
}

// Slicing one fruit should give one point
TEST_CASE("FRUIT_SLICE")
{
    Model model(config);
    model.game_over = false;
    Fruit new_fruit(Position(100,100), Velocity(0,0),0, config);
    model.fruits.push_back(new_fruit);
    CHECK(model.fruits[0].is_active());
    CHECK(model.get_score() == 0);
    model.set_mouse_pressed(true);
    model.set_mouse_position(Position(20,100).into<int>());
    model.on_frame(0.0000001);
    model.set_mouse_position(Position(100,100).into<int>());
    model.on_frame(0.0000001);
    CHECK(model.fruits[0].blade_entered);
    model.set_mouse_position(Position(200,100).into<int>());
    model.on_frame(0.0000001);
    CHECK(model.get_score() == 1);
}

// Slicing two fruits should give no combo points
TEST_CASE("SLICE_TWO_FRUIT")
{
    Model model(config);
    model.game_over = false;
    Fruit fruit1(Position(100,100), Velocity(0,0),0, config);
    Fruit fruit2(Position(300,100), Velocity(0,0),0, config);
    model.fruits.push_back(fruit1);
    model.fruits.push_back(fruit2);
    CHECK(model.fruits[0].is_active());
    CHECK(model.fruits[1].is_active());
    CHECK(model.get_score() == 0);
    model.set_mouse_pressed(true);
    model.set_mouse_position(Position(20,100).into<int>());
    model.on_frame(0.01);
    model.set_mouse_position(Position(100,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.fruits[0].blade_entered);
    model.set_mouse_position(Position(200,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.get_score() == 1);
    model.set_mouse_position(Position(300,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.fruits[1].blade_entered);
    model.set_mouse_position(Position(400,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.get_swipe_combo() == 2);
    model.set_mouse_pressed(false);
    model.on_frame(2);
    CHECK(model.get_score() == 2);
}

// Slicing three fruits should give combo points
TEST_CASE("SLICE_THREE_FRUIT_COMBO")
{
    Model model(config);
    model.game_over = false;
    Fruit fruit1(Position(100,100), Velocity(0,0),0, config);
    Fruit fruit2(Position(200,100), Velocity(0,0),0, config);
    Fruit fruit3(Position(300,100), Velocity(0,0),0, config);
    model.fruits.push_back(fruit1);
    model.fruits.push_back(fruit2);
    model.fruits.push_back(fruit3);
    model.set_mouse_pressed(true);
    model.increase_score(75);
    CHECK(model.get_score() == 75);
    CHECK(model.get_swipe_combo() == 0);
    model.set_mouse_position(Position(20,100).into<int>());
    model.on_frame(0.01);
    model.set_mouse_position(Position(100,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.fruits[0].blade_entered);
    model.set_mouse_position(Position(200,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.get_swipe_combo() == 1);
    CHECK(model.fruits[1].blade_entered);
    model.set_mouse_position(Position(300,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.get_score() == 77);
    CHECK(model.fruits[2].blade_entered);
    CHECK(model.get_swipe_combo() == 2);
    model.set_mouse_position(Position(400,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.get_swipe_combo() == 3);
    model.set_mouse_pressed(false);
    model.on_frame(2);
    // Gain 3 points and 2^3 bonus points
    CHECK(model.get_score() == 86);

}

// Slicing four fruits should give combo points
TEST_CASE("SLICE_FOUR_FRUIT_COMBO")
{
    Model model(config);
    model.game_over = false;
    Fruit fruit1(Position(100,100), Velocity(0,0),0, config);
    Fruit fruit2(Position(100,100), Velocity(0,0),0, config);
    Fruit fruit3(Position(100,100), Velocity(0,0),0, config);
    Fruit fruit4(Position(100,100), Velocity(0,0),0, config);
    model.fruits.push_back(fruit1);
    model.fruits.push_back(fruit2);
    model.fruits.push_back(fruit3);
    model.fruits.push_back(fruit4);
    model.set_mouse_pressed(true);
    model.increase_score(75);
    CHECK(model.get_score() == 75);
    model.set_mouse_position(Position(10,100).into<int>());
    model.on_frame(0.01);
    model.set_mouse_position(Position(100,100).into<int>());
    model.on_frame(0.01);
    CHECK(model.fruits[0].blade_entered);
    CHECK(model.fruits[1].blade_entered);
    CHECK(model.fruits[2].blade_entered);
    CHECK(model.fruits[3].blade_entered);
    model.set_mouse_position(Position(200,100).into<int>());
    model.on_frame(0.01);
    model.set_mouse_pressed(false);
    model.on_frame(2);
    // Gain 4 points and 2^4 bonus points
    CHECK(model.get_score() == 95);
}