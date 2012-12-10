#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "level.h"

#include "player.h"
#include "ui.h"
#include "ai.h"

#include "boring.h"

int play()
{
    int i;
    int turn;

    while (1)
    {
        explore();

        draw_map(current_level);
        draw_stats(player, current_level);

        if (player->hp <= 0)
            return prompt_yn("You died. Play again?");

        turn = player_turn();

        if (turn == 1)
            return 0;
        else if (turn == 2) {
            level_t * new_level = create_new_level(current_level);
            player = &new_level->mobs[0];

            boring_level(new_level, &player->x, &player->y);

            player->hp = current_level->mobs[0].hp;

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
    level_t * new_level = create_new_level(NULL);

    player = &new_level->mobs[0];
    current_level = new_level;

    boring_level(new_level, &player->x, &player->y);

    player->hp = 20;

    return;
}
