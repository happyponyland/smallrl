#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "level.h"

#include "player.h"
#include "ui.h"
#include "ai.h"
#include "item.h"

#include "standard_dungeon.h"
#include "los.h"


int play()
{
    int i;
    int turn;

    while (1)
    {
        explore();
        explore_map(current_level, player.mob->position);

        center_view(current_level, player.mob->position);

        draw_map(current_level);
        draw_stats(current_level);
        draw_log(current_level);

        if (player.mob->attr[ATTR_HP] <= 0)
            return prompt_yn("You died. Play again?");

        turn = player_turn();

        if (turn == turn_command_quit)
            return 0;
        else if (turn == turn_command_descend) {
            level_t * new_level = create_new_level(current_level, 80, 160);

            memcpy(&new_level->mobs[0],
                   player.mob,
                   sizeof(mob_t));

            player.mob = &new_level->mobs[0];

            create_standard_dungeon(new_level, &player.mob->position);

            free(current_level);

            current_level = new_level;

            continue; // give player the first turn on a new level
        }

        for (i = 1; i < MAX_MOBS_PER_LEVEL; i++)
        {
            enemy_turn(current_level, &current_level->mobs[i]);
        }
    }

    return 0;
}


void new_game()
{
    level_t * new_level = create_new_level(NULL, 80, 160);

    player.mob = &new_level->mobs[0];
    current_level = new_level;
    current_ui_input_type = ui_input_type_map;

    reset_log();

    create_standard_dungeon(new_level, &player.mob->position);

    player.level = 1;
    player.exp = 0;

    memset(&player.inventory, 0, sizeof(item_t *) * INVENTORY_SIZE);

    player.mob->attr[ATTR_HP] = 20;
    player.mob->attr[ATTR_ATTACK] = 20;
    player.mob->attr[ATTR_DODGE] = 20;
    player.mob->attr[ATTR_MINDAM] = 1;
    player.mob->attr[ATTR_MAXDAM] = 5;

    size_t i;
    for (i = 0; i < INVENTORY_SIZE; i++)
    {
        if(player.inventory[i] != NULL) {
            free(player.inventory[i]);
            player.inventory[i] = NULL;
        }
    }

    return;
}
