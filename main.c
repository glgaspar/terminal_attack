#include "terminalattack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

void add_enemy(struct Character *enemy, struct Character **enemy_list) {
    bool added = false;
    for (int i = 0; i < MAX_ENEMY_AMOUNT; i++) {
        if (enemy_list[i] == NULL) {
            enemy_list[i] = enemy;
            added = true;
            break;
        }
    }
    if (!added) {
        free(enemy);
    }
}

void remove_enemy(struct Character *enemy, struct Character **enemy_list){
    for (int i = 0; i < MAX_ENEMY_AMOUNT; i++) {
        if (enemy_list[i] == enemy) {
            enemy_list[i] = NULL;
            break;
        }
    }
    free(enemy->movable);
    free(enemy);
}

void update_bullets(struct Bullet **bullet_list) {
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (bullet_list[i] != NULL) {
            bullet_list[i]->movable->object_move(bullet_list[i]->movable->move_speed, 'y', bullet_list[i]->movable);
            if (bullet_list[i]->movable->y_axis < ENEMY_SPAWN_Y_AXIS || bullet_list[i]->movable->y_axis > PLAYER_SPAWN_Y_AXIS) {
                free(bullet_list[i]->movable);
                free(bullet_list[i]);
                bullet_list[i] = NULL;
            }
        }
    }
}

void add_bullet(struct Bullet *bullet, struct Bullet **bullet_list) {
    bool added = false;
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (bullet_list[i] == NULL) {
            bullet_list[i] = bullet;
            added = true;
            break;
        }
    }
    if (!added) {
        free(bullet);
    }
}

void player_actions(int key, struct Character *player, struct Bullet **bullet_list) {
    switch (key) {
        case KEY_UP:
            player->movable->object_move(-player->movable->move_speed, 'y', player->movable);
            break;
        case KEY_DOWN:
            player->movable->object_move(player->movable->move_speed, 'y', player->movable);
            break;
        case KEY_LEFT:
            player->movable->object_move(-player->movable->move_speed, 'x', player->movable);
            break;
        case KEY_RIGHT:
            player->movable->object_move(player->movable->move_speed, 'x', player->movable);
            break;
        case ' ':
            {
                struct Bullet *bullet = player->shoot(player);
                bullet->movable->move_speed = -bullet->movable->move_speed; 
                add_bullet(bullet, bullet_list);
            }
            break;
        default:
            break;
    }
}

void update_enemies(struct Character **enemy_list) {
    for (int i = 0; i < MAX_ENEMY_AMOUNT; i++) {
        if (enemy_list[i] != NULL) {
            if (enemy_list[i]->movable->x_axis >= MAX_GRID_X || enemy_list[i]->movable->x_axis <= 0) {
                enemy_list[i]->movable->move_speed = -enemy_list[i]->movable->move_speed;
            }
            enemy_list[i]->movable->object_move(enemy_list[i]->movable->move_speed, 'x', enemy_list[i]->movable);
        }
    }
}

void draw(struct Character *player, struct Character **enemy_list, struct Bullet **bullet_list) {
    mvprintw(player->movable->y_axis, player->movable->x_axis, "P");
    
    for (int i = 0; i < MAX_ENEMY_AMOUNT; i++) {
        if (enemy_list[i] != NULL) {
            mvprintw(enemy_list[i]->movable->y_axis, enemy_list[i]->movable->x_axis, "E");
        }
    }
    
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (bullet_list[i] != NULL) {
            mvprintw(bullet_list[i]->movable->y_axis, bullet_list[i]->movable->x_axis, "*");
        }
    }
}

int main(){
    struct Character **enemy_list = calloc(MAX_ENEMY_AMOUNT, sizeof(struct Character *));
    struct Character *player;
    time_t last_enemy_spawn;
    time_t current_time;
    struct Bullet **bullet_list = calloc(MAX_BULLET_AMOUNT, sizeof(struct Bullet *));
    
    initscr();
    cbreak();
    noecho();
    
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(NULL));


    time(&last_enemy_spawn);
    player = character_constructor(PLAYER_SPAWN_Y_AXIS, PLAYER_SPAWN_X_AXIS, PLAYER_HEALTH_POINTS, PLAYER_MOVE_SPEED, PLAYER_DAMAGE_AMOUNT, PLAYER_BULLET_SPEED);
    
    
    while (true) {
        int key = getch();
        player_actions(key, player, bullet_list);

        time(&current_time);

        int enemy_selector = rand() % MAX_ENEMY_AMOUNT;
        
        if (enemy_list[enemy_selector] != NULL) {
            struct Bullet *bullet = enemy_list[enemy_selector]->shoot(enemy_list[enemy_selector]);
            add_bullet(bullet, bullet_list);
        }

        if (difftime(current_time, last_enemy_spawn) >= ENEMY_SPAWN_INTERVAL) {
            struct Character *enemy = character_constructor(ENEMY_SPAWN_Y_AXIS, ENEMY_SPAWN_X_AXIS, ENEMY_HEALTH_POINTS, ENEMY_MOVE_SPEED, ENEMY_DAMAGE_AMOUNT, ENEMY_BULLET_SPEED);
            add_enemy(enemy, enemy_list);
            last_enemy_spawn = current_time;
        }
        

        update_enemies(enemy_list);

        update_bullets(bullet_list);

        clear();
        draw(player, enemy_list, bullet_list);
        refresh();
        usleep(300000);
    }

    return 0;
}