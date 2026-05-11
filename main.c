#include "terminalattack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
        printf("Enemy list is full. Cannot add more enemies.\n");
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
    free(enemy);
}

void update_bullets(struct Bullet **bullet_list) {
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (bullet_list[i] != NULL) {
            bullet_list[i]->movable->move(bullet_list[i]->movable->move_speed, 'y', bullet_list[i]->movable);
            if (bullet_list[i]->movable->y_axis > ENEMY_SPAWN_Y_AXIS) {
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
        printf("Bullet list is full. Cannot add more bullets.\n");
        free(bullet);
    }
}


int main(){
    struct Character **enemy_list = calloc(MAX_ENEMY_AMOUNT, sizeof(struct Character));
    struct Character *player;
    time_t last_enemy_spawn;
    time_t current_time;
    struct Bullet **bullet_list = calloc(MAX_BULLET_AMOUNT, sizeof(struct Bullet));


    time(&last_enemy_spawn);
    player = character_constructor(0, 0, PLAYER_HEALTH_POINTS, PLAYER_MOVE_SPEED, PLAYER_DAMAGE_AMOUNT, PLAYER_BULLET_SPEED);
    
    
    while (true) {
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

        update_bullets(bullet_list);

        
    }

    return 0;
}