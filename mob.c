#include <stdlib.h>

#include <string.h>

#include "mob.h"

static void mob_adjective_magician(char *);
static void mob_adjective_default(char *);

void mob_name(char * s, mob_type_t type)
{
    switch (type)
    {
        case mob_player:
            strcpy(s, "player");
            break;

        case mob_magician:
            strcpy(s, "magician");
            break;

        case mob_newbie:
            strcpy(s, "newbie");
            break;

        case mob_zombie:
            strcpy(s, "zombie");
            break;

        case mob_1337hax0r:
            strcpy(s, "1337-hax0r");
            break;

        case mob_hobbyist:
            strcpy(s, "hobbyist programmer");
            break;

        case mob_war_troll:
            strcpy(s, "war-troll");
            break;

        case mob_police:
            strcpy(s, "police officer");
            break;

        default:
            strcpy(s, "???");
            break;
    }

    return;
}



void mob_adjective(char * s, mob_type_t type)
{
    switch(type)
    {
        case mob_magician:
            mob_adjective_magician(s);
            break;
        default:
            mob_adjective_default(s);
    }
}

static void mob_adjective_magician(char * s)
{
    switch (rand() % 10)
    {
        case 0:
            strcpy(s, "ancient ");
            break;

        case 1:
            strcpy(s, "powerful ");
            break;

        case 2:
            strcpy(s, "strange looking ");
            break;

        case 3:
            strcpy(s, "dark ");
            break;

        default:
            strcpy(s, "");
            break;
    }
}

static void mob_adjective_default(char * s)
{
    switch (rand() % 20)
    {
        case 0:
            strcpy(s, "evil ");
            break;

        case 1:
            strcpy(s, "ugly ");
            break;

        case 2:
            strcpy(s, "stupid ");
            break;

        case 3:
            strcpy(s, "silly ");
            break;

        case 4:
            strcpy(s, "grisly ");
            break;

        case 5:
            strcpy(s, "sweaty ");
            break;

        case 6:
            strcpy(s, "smelly ");
            break;

        default:
            strcpy(s, "");
            break;
    }
}
