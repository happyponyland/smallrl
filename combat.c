#include "smallrl.h"

void melee(int att, int def)
{
    char line[100];
    char att_n[50];
    char def_n[50];

    mob_name(att_n, mob[att].type);
    mob_name(def_n, mob[def].type);

    mob[def].hp -= rand() % 4;

    sprintf(line, "The %s hits the %s!", att_n, def_n);
    print_msg(line);
    wait();
    clear_msg();

    if (mob[def].hp <= 0)
    {
        sprintf(line, "The %s dies!", def_n);
        print_msg(line);
        mob[def].type = mob_none;
    }

    return;
}



