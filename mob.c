#include <stdlib.h>

#include <string.h>

#include "mob.h"

static void mob_adjective_magician(char *);
static void mob_adjective_default(char *);

void mob_name(mob_type_t mob_type, char * out_string)
{
    switch (mob_type)
    {
        case mob_player:
            strcpy(out_string, "player");
            break;

        case mob_magician:
            strcpy(out_string, "magician");
            break;

        case mob_newbie:
            strcpy(out_string, "newbie");
            break;

        case mob_zombie:
            strcpy(out_string, "zombie");
            break;

        case mob_1337hax0r:
            strcpy(out_string, "1337-hax0r");
            break;

        case mob_hobbyist:
            strcpy(out_string, "hobbyist programmer");
            break;

        case mob_war_troll:
            strcpy(out_string, "war-troll");
            break;

        case mob_police:
            strcpy(out_string, "police officer");
            break;

        default:
            strcpy(out_string, "placeholder");
            break;
    }

    return;
}



void mob_adjective(mob_type_t mob_type, char * out_string)
{
    switch(mob_type)
    {
        case mob_magician:
            mob_adjective_magician(out_string);
            break;
        default:
            mob_adjective_default(out_string);
    }
}

static void mob_adjective_magician(char * out_string)
{
    switch (rand() % 10)
    {
        case 0:
            strcpy(out_string, "ancient ");
            break;

        case 1:
            strcpy(out_string, "powerful ");
            break;

        case 2:
            strcpy(out_string, "strange looking ");
            break;

        case 3:
            strcpy(out_string, "dark ");
            break;

        default:
            strcpy(out_string, "");
            break;
    }
}

static void mob_adjective_default(char * out_string)
{
    switch (rand() % 20)
    {
        case 0:
            strcpy(out_string, "evil ");
            break;

        case 1:
            strcpy(out_string, "ugly ");
            break;

        case 2:
            strcpy(out_string, "stupid ");
            break;

        case 3:
            strcpy(out_string, "silly ");
            break;

        case 4:
            strcpy(out_string, "grisly ");
            break;

        case 5:
            strcpy(out_string, "sweaty ");
            break;

        case 6:
            strcpy(out_string, "smelly ");
            break;

        default:
            strcpy(out_string, "");
            break;
    }
}
