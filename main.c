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

void update_bullets(struct Bullet **bullet) {
    (*bullet)->movable->object_move((*bullet)->movable->move_speed, 'y', (*bullet)->movable);
    if ((*bullet)->movable->y_axis < ENEMY_SPAWN_Y_AXIS || (*bullet)->movable->y_axis > PLAYER_SPAWN_Y_AXIS) {
        free((*bullet)->movable);
        free(*bullet);
        *bullet = NULL;
    }
}

void check_for_collision(struct Bullet **bullet, struct Character *character) {
    if ((*bullet)->movable->y_axis == character->movable->y_axis && (*bullet)->movable->x_axis == character->movable->x_axis) {
        character->take_damage((*bullet)->damage_amount, character);
        free((*bullet)->movable);
        free(*bullet);
        *bullet = NULL;
    }
}

void update_player_bullets(struct Bullet **bullet_list, struct Character **enemy_list) {
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (bullet_list[i] == NULL) {
            continue;
        }
        update_bullets(&bullet_list[i]); 
        if (bullet_list[i] == NULL) {
            continue;
        }
        for (int j = 0; j < MAX_ENEMY_AMOUNT; j++) {
            if (enemy_list[j] == NULL) {
                continue;
            }
            check_for_collision(&bullet_list[i], enemy_list[j]);
            if (bullet_list[i] == NULL) {
                break; 
            }
        }
    }
}

void update_enemy_bullets(struct Bullet **bullet_list, struct Character *player) {
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (bullet_list[i] == NULL) {
            continue;
        }
        update_bullets(&bullet_list[i]); 
        if (bullet_list[i] == NULL) {
            continue;
        }
        check_for_collision(&bullet_list[i], player);
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
        if (enemy_list[i] == NULL) {
            continue;
        }
        if (enemy_list[i]->health_points <= 0) {
            remove_enemy(enemy_list[i], enemy_list);
            continue;
        }
        if (enemy_list[i]->movable->x_axis >= MAX_GRID_X || enemy_list[i]->movable->x_axis <= 0) {
            enemy_list[i]->movable->move_speed = -enemy_list[i]->movable->move_speed;
        }
        enemy_list[i]->movable->object_move(enemy_list[i]->movable->move_speed, 'x', enemy_list[i]->movable);
    }
}

void draw(struct Character *player, struct Character **enemy_list, struct Bullet **enemy_bullet_list, struct Bullet **player_bullet_list) {
    mvprintw(player->movable->y_axis, player->movable->x_axis, "P");
    
    for (int i = 0; i < MAX_ENEMY_AMOUNT; i++) {
        if (enemy_list[i] == NULL) {
            continue;
        }
        mvprintw(enemy_list[i]->movable->y_axis, enemy_list[i]->movable->x_axis, "E");
    }
    
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (enemy_bullet_list[i] == NULL) {
            continue;
        }
        mvprintw(enemy_bullet_list[i]->movable->y_axis, enemy_bullet_list[i]->movable->x_axis, "*");
    }
    
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (player_bullet_list[i] == NULL) {
            continue;
        }
        mvprintw(player_bullet_list[i]->movable->y_axis, player_bullet_list[i]->movable->x_axis, "*");
    }
}

void show_home_screen() {
    clear();
    // Draw the grid border
    for (int x = 0; x <= MAX_GRID_X; x++) {
        mvprintw(0, x, "-");
        mvprintw(MAX_GRID_Y, x, "-");
    }
    for (int y = 0; y <= MAX_GRID_Y; y++) {
        mvprintw(y, 0, "|");
        mvprintw(y, MAX_GRID_X, "|");
    }
    mvprintw(0, 0, "+");
    mvprintw(0, MAX_GRID_X, "+");
    mvprintw(MAX_GRID_Y, 0, "+");
    mvprintw(MAX_GRID_Y, MAX_GRID_X, "+");

    mvprintw(MAX_GRID_Y / 2 - 2, 0, "TERMINAL ATTACK");
    mvprintw(MAX_GRID_Y / 2, 0, "Press any key");
    mvprintw(MAX_GRID_Y / 2 + 1, 0, "to start...");
    refresh();
    nodelay(stdscr, FALSE); // Temporarily make getch() blocking
    getch();                // Wait for the user to press a key
    nodelay(stdscr, TRUE);  // Restore non-blocking input for the game
}

int main(){
    struct Character **enemy_list = calloc(MAX_ENEMY_AMOUNT, sizeof(struct Character *));
    struct Character *player;
    time_t last_enemy_spawn;
    time_t current_time;
    struct Bullet **enemy_bullet_list = calloc(MAX_BULLET_AMOUNT, sizeof(struct Bullet *));
    struct Bullet **player_bullet_list = calloc(MAX_BULLET_AMOUNT, sizeof(struct Bullet *));
    
    initscr();
    cbreak();
    noecho();
    
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(NULL));

    show_home_screen();

    time(&last_enemy_spawn);
    player = character_constructor(PLAYER_SPAWN_Y_AXIS, PLAYER_SPAWN_X_AXIS, PLAYER_HEALTH_POINTS, PLAYER_MOVE_SPEED, PLAYER_DAMAGE_AMOUNT, PLAYER_BULLET_SPEED);
    
    
    while (true) {
        int key = getch();
        player_actions(key, player, player_bullet_list);

        time(&current_time);

        int enemy_selector = rand() % MAX_ENEMY_AMOUNT;
        
        if (enemy_list[enemy_selector] != NULL) {
            struct Bullet *bullet = enemy_list[enemy_selector]->shoot(enemy_list[enemy_selector]);
            add_bullet(bullet, enemy_bullet_list);
        }

        if (difftime(current_time, last_enemy_spawn) >= ENEMY_SPAWN_INTERVAL) {
            struct Character *enemy = character_constructor(ENEMY_SPAWN_Y_AXIS, ENEMY_SPAWN_X_AXIS, ENEMY_HEALTH_POINTS, ENEMY_MOVE_SPEED, ENEMY_DAMAGE_AMOUNT, ENEMY_BULLET_SPEED);
            add_enemy(enemy, enemy_list);
            last_enemy_spawn = current_time;
        }
        

        update_player_bullets(player_bullet_list, enemy_list);
        update_enemy_bullets(enemy_bullet_list, player);

        if (player->health_points <= 0) {
            break;
        }
        update_enemies(enemy_list);


        clear();
        draw(player, enemy_list, enemy_bullet_list, player_bullet_list  );
        refresh();
        usleep(300000);
    }

    clear();

    free(player->movable);
    free(player);
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (enemy_bullet_list[i] != NULL) {
            free(enemy_bullet_list[i]->movable);
            free(enemy_bullet_list[i]);
        }
        if (player_bullet_list[i] != NULL) {
            free(player_bullet_list[i]->movable);
            free(player_bullet_list[i]);
        }
    }
    free(enemy_bullet_list);
    free(player_bullet_list);
    for (int i = 0; i < MAX_ENEMY_AMOUNT; i++) {
        if (enemy_list[i] != NULL) {
            free(enemy_list[i]->movable);
            free(enemy_list[i]);
        }
    }
    free(enemy_list);

    endwin();

    return 0;
}