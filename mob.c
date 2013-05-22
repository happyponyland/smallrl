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

        case mob_rat:
            strcpy(out_string, "rat");
            break;

        case mob_zombie:
            strcpy(out_string, "zombie");
            break;

        case mob_magician:
            strcpy(out_string, "magician");
            break;

        case mob_giant_rat:
            strcpy(out_string, "giant rat");
            break;

        case mob_boreworm:
            strcpy(out_string, "boreworm");
            break;

        case mob_green_bobling:
            strcpy(out_string, "green bobling");
            break;

        case mob_grey_bobling:
            strcpy(out_string, "grey bobling");
            break;

        case mob_war_troll:
            strcpy(out_string, "war-troll");
            break;

        case mob_demon:
            strcpy(out_string, "demon");
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
            strcpy(out_string, "sinister ");
            break;

        case 1:
            strcpy(out_string, "ugly ");
            break;

        case 2:
            strcpy(out_string, "warty ");
            break;

        case 3:
            strcpy(out_string, "limping ");
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
