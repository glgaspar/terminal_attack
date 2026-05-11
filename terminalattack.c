#include "terminalattack.h"

void take_damage(int damage_amount, struct Character *character) {
    character->health_points -= damage_amount;
}

struct Bullet *shoot(struct Character *character) {
    return bullet_constructor(character->movable->y_axis, character->movable->x_axis, character->bullet_speed, character->damage_amount);
}

void object_move(int speed, char axis, struct Movable *movable) {
    if (axis == 'x') {
        movable->x_axis += speed;
    } else {
        movable->y_axis += speed;
    }
}

struct Movable *movable_constructor(int y_axis, int x_axis, int move_speed) {
    struct Movable *movable = malloc(sizeof(struct Movable));
    movable->y_axis = y_axis;
    movable->x_axis = x_axis;
    movable->move_speed = move_speed;
    movable->object_move = &object_move;

    return movable;
};


struct Bullet *bullet_constructor(int y_axis, int x_axis, int move_speed, int damage_amount) {
    struct Bullet *bullet = malloc(sizeof(struct Bullet));
    bullet->damage_amount = damage_amount;

    struct Movable *movable;
    movable = movable_constructor(y_axis, x_axis, move_speed);
    bullet->movable = movable;
    

    return bullet;
};


struct Character *character_constructor(int y_axis, int x_axis, int health_points, int move_speed, int damage_amount, int bullet_speed) {
    struct Character *character = malloc(sizeof(struct Character));
    character->health_points = health_points;
    character->damage_amount = damage_amount;
    character->bullet_speed = bullet_speed;
    character->take_damage = &take_damage;
    character->shoot = &shoot;

    struct Movable *movable;
    movable = movable_constructor(y_axis, x_axis, move_speed);
    character->movable = movable;
    

    return character;
};
