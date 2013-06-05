#include <stdlib.h>
#include <stdio.h>

#include "combat.h"
#include "player.h"
#include "mob.h"
#include "ui.h"
#include "chaos.h"

static void melee(mob_t *, mob_t *);

void attack(mob_t * attacker, mob_t * defender)
{
    melee(attacker, defender);
}

static void melee(mob_t * attacker, mob_t * defender)
{
    char line[MSGLEN];
    int def_dodge;
    int att_skill;
    int damage;
    char * att_a,  * att_n, * def_a, * def_n;

    att_a = attacker->description_adjective;
    att_n = attacker->description_noun;

    def_a = defender->description_adjective;
    def_n = defender->description_noun;

    if(attacker->type == mob_magician && defender->type == mob_player)
    {
        snprintf(line, MSGLEN,
                 "The %s%s challenges you to a duel.",
                 att_a, att_n);
        print_msg(line);
        wait();
        clear_msg();

        chaos_outcome_t duel_outcome = chaos_duel();

        if(duel_outcome == chaos_outcome_win) {
            snprintf(line, MSGLEN, "The %s%s holds his hands against his head and screams \"NOOOOoooOOOoooooo!!\" before vanishing in a puff of smoke.", att_a, att_n);

            /* show that the mob is gone before printing message*/
            attacker->type = mob_none;
            draw_map(current_level);

            print_msg(line);
            wait();
            clear_msg();

            //TODO: Use multiplier from number of chaos opponents?
            give_exp(defender->attr[ATTR_EXP]);
        } else if(duel_outcome == chaos_outcome_loss) {
            defender->attr[ATTR_HP] = 0;
            print_msg("You did not survive the duel!!");
            wait();
            return;
        }

        return;
    }

    if(defender->is_immortal)
    {
        snprintf(line, MSGLEN,
                 "The %s%s smiles at your feeble attack.",
                 def_a, def_n);
        print_msg(line);
        wait();
        clear_msg();
        return;
    }

    if ((attacker->type == mob_rat
         || attacker->type == mob_giant_rat)
        && rand() % 5 == 0)
    {
        snprintf(line, MSGLEN,
                 "The %s%s nibbles on your toes.!",
                 att_a, att_n);
        print_msg(line);
        wait();

        if (player.exp > 50)
            player.exp -= 50;
        else
            player.exp = 0;

        draw_stats(current_level);

        clear_msg();
        return;
    }

    /* Hit/miss roll */
    att_skill = attacker->attr[ATTR_ATTACK];
    def_dodge = attacker->attr[ATTR_DODGE];

    if ((rand() % 100) < (def_dodge - att_skill) + 1)
    {
        if (attacker == player.mob)
            snprintf(line, MSGLEN, "You miss the %s%s!", def_a, def_n);
        else if (defender == player.mob)
            snprintf(line, MSGLEN, "The %s%s misses you!", att_a, att_n);
        else
            snprintf(line, MSGLEN, "The %s%s misses the %s%s!",
                     att_a, att_n, def_a, def_n);

        print_msg(line);
        wait();
        clear_msg();
        return;
    }

    /* It was a hit */
    damage = attacker->attr[ATTR_MINDAM] +
        (rand() % (attacker->attr[ATTR_MAXDAM] + 1));

    defender->attr[ATTR_HP] -= damage;

    if (attacker == player.mob)
        snprintf(line, MSGLEN, "You hit the %s%s! [%d]",
                 def_a, def_n, damage);
    else if (defender == player.mob)
        snprintf(line, MSGLEN, "The %s%s hits you! [%d]",
                 att_a, att_n, damage);
    else
        snprintf(line, MSGLEN, "The %s%s hits the %s%s! [%d]",
                 att_a, att_n, def_a, def_n, damage);

    print_msg(line);
    wait();
    clear_msg();

    if (defender->attr[ATTR_HP] <= 0)
    {
        if (defender == player.mob)
        {
            print_msg("You have been slain!!");
            wait();
            return;
        }

        snprintf(line, MSGLEN, "The %s%s dies!", def_a, def_n);

        /* show that the mob is gone before printing message*/
        defender->type = mob_none;
        draw_map(current_level);

        print_msg(line);
        wait();
        clear_msg();

        if (attacker == player.mob)
        {
            give_exp(defender->attr[ATTR_EXP]);
        }
    }

    return;
}
