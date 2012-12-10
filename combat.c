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

    mob_name(att_n, attacker->type);
    mob_name(def_n, defender->type);
    mob_adjective(att_a, attacker->type);
    mob_adjective(def_a, defender->type);
    
    defender->hp -= 1 + (rand() % 4);

    if (attacker == player)
        snprintf(line, MSGLEN, "You hit the %s%s!", def_a, def_n);
    else if (defender == player)
        snprintf(line, MSGLEN, "The %s%s hits you!", att_a, att_n);
    else
        snprintf(line, MSGLEN, "The %s%s hits the %s%s!",
                 att_a, att_n, def_a, def_n);

    print_msg(line);
    wait();
    clear_msg();

    if (defender->hp <= 0)
    {
        if (defender == player)
        {
            print_msg("You have been slain!!");
            return;
        }
        
        sprintf(line, "The %s dies!", def_n);
        print_msg(line);
        defender->type = mob_none;
    }

    return;
}
