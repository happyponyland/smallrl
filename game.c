#include <stdlib.h>
#include <string.h>

#include "game.h"

#include "ui.h"
#include "player.h"
#include "level.h"
#include "log.h"
#include "mob.h"

#include "level.h"

#include "player.h"
#include "ui.h"
#include "ai.h"
#include "item.h"
#include "combat.h"

#include "standard_dungeon.h"
#include "los.h"
#include "log.h"

int play(game_t * game)
{
    int i;
    int turn;

    while (1)
    {
        explore(game->level, game->player.mob);
        explore_map(game->level, game->player.mob->position);

        center_view(game->level, game->player.mob->position);

        draw_map(game->input_type, game->level);
        draw_stats(&(game->player), game->level);
        draw_log(game->input_type, &(game->log), game->level);

        if (game->player.mob->attr[ATTR_HP] <= 0)
            return prompt_yn("You died. Play again?");

        turn = player_turn(game);

        if (turn == turn_command_quit)
            return 0;
        else if (turn == turn_command_descend) {
            level_t * new_level = create_new_level(game->level, 80, 160);

            memcpy(&new_level->mobs[0],
                   game->player.mob,
                   sizeof(mob_t));

            game->player.mob = &new_level->mobs[0];

            create_standard_dungeon(new_level, &(game->player.mob->position));

            game->level = new_level;

            continue; // give player the first turn on a new level
        }

        for (i = 1; i < MAX_MOBS_PER_LEVEL; i++)
        {
            enemy_turn(game, game->level, &game->level->mobs[i]);
        }
    }

    return 0;
}

void free_game(game_t * game)
{
    free(game->level);
    free(game);

    return;
}

game_t * new_game()
{
    game_t * game = malloc(sizeof(game_t));

    game->player.mob = NULL,
    game->player.level = 1,
    game->player.exp = 0,

    memset(game->player.inventory, 0, INVENTORY_SIZE * sizeof(item_t *));

    level_t * new_level = create_new_level(NULL, 80, 160);

    game->player.mob = &new_level->mobs[0];

    game->level = new_level;

    game->input_type = input_type_map;

    init_log(&game->log);

    create_standard_dungeon(new_level, &game->player.mob->position);

    game->player.level = 1;
    game->player.exp = 0;

    game->player.mob->attr[ATTR_HP] = 20;
    game->player.mob->attr[ATTR_ATTACK] = 20;
    game->player.mob->attr[ATTR_DODGE] = 20;
    game->player.mob->attr[ATTR_MINDAM] = 1;
    game->player.mob->attr[ATTR_MAXDAM] = 5;

    size_t i;
    for (i = 0; i < INVENTORY_SIZE; i++)
    {
        if(game->player.inventory[i] != NULL) {
            free(game->player.inventory[i]);
            game->player.inventory[i] = NULL;
        }
    }

    *(game->log.log_input) = '\0';

    return game;
}

bool try_move_mob(game_t * game, level_t * level, mob_t * mob_to_move, int y_speed, int x_speed)
{
    int new_y = mob_to_move->position.y + y_speed;
    int new_x = mob_to_move->position.x + x_speed;

    if (on_map(level, new_y, new_x) &&
        !(level->map[new_y * level->width + new_x].flags & tile_unpassable))
    {
        if(new_y == game->player.mob->position.y && new_x == game->player.mob->position.x)
        {
            attack(game, mob_to_move, game->player.mob);
            return 0;
        }

        for(int i = 1; i < MAX_MOBS_PER_LEVEL; ++i)
        {
            if(level->mobs[i].type == mob_none)
                continue;

            if(new_y == level->mobs[i].position.y && new_x == level->mobs[i].position.x)
                return 0;
        }

        mob_to_move->position.y = new_y;
        mob_to_move->position.x = new_x;

        return true;
    }

    return false;
}

turn_command_t execute_command(game_t * game, char * command)
{
    game_t * new_game = game;
    game = new_game;

    if(strcmp("quit", command) == 0) {
        return turn_command_quit;
    }

    return turn_command_complete;
}
