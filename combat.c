#include <stdlib.h>
#include <stdio.h>

#include "combat.h"
#include "player.h"
#include "mob.h"
#include "ui.h"

static void melee(mob_t * attacker, mob_t * defender);

void attack(mob_t * attacker, mob_t * defender)
{
    melee(attacker, defender);
}

static void melee(mob_t * attacker, mob_t * defender)
{
    char line[MSGLEN];
    char att_n[50];
    char def_n[50];
    char att_a[50];
    char def_a[50];
    int def_dodge;
    int att_skill;
    int damage;

    mob_name(att_n, attacker->type);
    mob_name(def_n, defender->type);
    mob_adjective(att_a, attacker->type);
    mob_adjective(def_a, defender->type);
    
    /* Hit/miss roll */
    att_skill = attacker->attr[ATTR_ATTACK];
    def_dodge = attacker->attr[ATTR_DODGE];

    if ((rand() % 100) < (def_dodge - att_skill) + 1)
    {
        if (attacker == player)
            snprintf(line, MSGLEN, "You miss the %s%s!", def_a, def_n);
        else if (defender == player)
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

    if (attacker == player)
        snprintf(line, MSGLEN, "You hit the %s%s! [%d]",
                 def_a, def_n, damage);
    else if (defender == player)
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
        if (defender == player)
        {
            print_msg("You have been slain!!");
            wait();
            return;
        }
        
        mob_adjective(def_a, defender->type);
        snprintf(line, MSGLEN, "The %s%s dies!", def_a, def_n);

        /* show that the mob is gone before printing message*/
        defender->type = mob_none;
        draw_map(current_level);

        print_msg(line);
        wait();
        clear_msg();

        if (attacker == player)
        {
            give_exp(defender->attr[ATTR_EXP]);
        }
    }

    return;
}
