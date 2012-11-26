#include <stdlib.h>
#include <stdio.h>

#include "combat.h"
#include "mob.h"
#include "ui.h"


void melee(mob_t * attacker, mob_t * defender)
{
    char line[100];
    char att_n[50];
    char def_n[50];

    mob_name(att_n, attacker->type);
    mob_name(def_n, defender->type);

    defender->hp -= rand() % 4;

    sprintf(line, "The %s hits the %s!", att_n, def_n);
    print_msg(line);
    wait();
    clear_msg();

    if (defender->hp <= 0)
    {
        sprintf(line, "The %s dies!", def_n);
        print_msg(line);
        defender->type = mob_none;
    }

    return;
}
