#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "level.h"

#include "player.h"
#include "ui.h"
#include "ai.h"
#include "item.h"

#include "standard_dungeon.h"

int play()
{
    int i;
    int turn;

    while (1)
    {
        explore();

        center_view(current_level, player.mob->position);

        draw_map(current_level);
        draw_stats();

        if (player.mob->attr[ATTR_HP] <= 0)
            return prompt_yn("You died. Play again?");

        turn = player_turn();

        if (turn == TURN_QUIT)
            return 0;
        else if (turn == TURN_DESCEND) {
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

    player.phone_status = 0;
    player.mob = &new_level->mobs[0];
    current_level = new_level;

    create_standard_dungeon(new_level, &player.mob->position);

    player.level = 1;
    player.exp = 0;

    player.mob->attr[ATTR_HP] = 20;
    player.mob->attr[ATTR_ATTACK] = 20;
    player.mob->attr[ATTR_DODGE] = 20;
    player.mob->attr[ATTR_MINDAM] = 1;
    player.mob->attr[ATTR_MAXDAM] = 5;

    size_t i;
    for (i = 0; i < INVENTORY_SIZE; i++)
        player.inventory[i] = 0;

    player.inventory[0] = item_dildo;
    player.inventory[1] = item_leather_pants;
    player.inventory[2] = item_med_prot;
    player.inventory[3] = item_healing_pot;
    player.inventory[4] = item_healing_pot;
    player.inventory[4] = item_telephone;

    return;
}
