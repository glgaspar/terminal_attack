#include <time.h>

#ifndef TERMINAL_ATTACK_H

#define TERMINAL_ATTACK_H
#define MAX_ENEMY_AMOUNT 5
#define ENEMY_SPAWN_INTERVAL 5
#define ENEMY_HEALTH_POINTS 5   
#define ENEMY_MOVE_SPEED 1
#define ENEMY_DAMAGE_AMOUNT 5
#define ENEMY_BULLET_SPEED 1
#define ENEMY_SPAWN_Y_AXIS 0
#define ENEMY_SPAWN_X_AXIS MAX_GRID_X/2
#define ENEMY_MOVE_COOLDOWN 3
#define ENEMY_SHOOT_COOLDOWN 2

#define PLAYER_HEALTH_POINTS 100
#define PLAYER_MOVE_SPEED 1
#define PLAYER_DAMAGE_AMOUNT 10
#define PLAYER_BULLET_SPEED 2
#define PLAYER_SPAWN_Y_AXIS MAX_GRID_Y-1
#define PLAYER_SPAWN_X_AXIS MAX_GRID_X/2
#define PLAYER_MOVE_COOLDOWN 0
#define PLAYER_SHOOT_COOLDOWN 1


#define MAX_BULLET_AMOUNT 100

#define MAX_GRID_X 10
#define MAX_GRID_Y 20

struct Bullet {
    struct Movable *movable;
    int damage_amount;
};

struct Character {
    struct Movable *movable;
    int health_points;
    int damage_amount;
    int bullet_speed;
    int move_cooldown;
    time_t last_move_time;
    int shoot_cooldown;
    time_t last_shoot_time;

    void (*take_damage)(int damage_amount, struct Character *character);
    struct Bullet *(*shoot)(struct Character *character);
};

struct Movable {
    int y_axis;
    int x_axis;
    int move_speed;

    void (*object_move)(int speed, char axis, struct Movable *movable);
};


struct Bullet *bullet_constructor(int y_axis, int x_axis, int move_speed, int damage_amount);
struct Character *character_constructor(int y_axis, int x_axis, int health_points, int move_speed, int damage_amount, int bullet_speed, int move_cooldown, int shoot_cooldown);
struct Movable *movable_constructor(int y_axis, int x_axis, int move_speed);

void take_damage(int damage_amount, struct Character *character);
struct Bullet *shoot(struct Character *character);

void object_move(int speed, char axis, struct Movable *movable);

#endif