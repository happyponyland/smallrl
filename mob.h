#ifndef SMALLRL_MOB_H
#define SMALLRL_MOB_H

#include <stdbool.h>
#include "types.h"

#define ATTR_HP      0
#define ATTR_MINDAM  1
#define ATTR_MAXDAM  2
#define ATTR_ATTACK  3
#define ATTR_DODGE   4
#define ATTR_EXP     5
#define ATTRIBUTES 15

typedef enum mob_type_e
{
    mob_none = 0,
    mob_player = '@',
    mob_magician = 'M',
    mob_rat = 'r',
    mob_giant_rat = 'R',
    mob_boreworm = 'b',
    mob_zombie = 'z',
    mob_green_bobling = 'g',
    mob_grey_bobling = 'G',
    mob_war_troll = 'T',
    mob_demon = 'd'
} mob_type_t;

typedef struct mob_s
{
    mob_type_t type;

    char description_adjective[50];
    char description_noun[50];

    point_t position;

    int attr[ATTRIBUTES];
    bool is_immortal;

    int turn_counter;
    int mana;
} mob_t;

void mob_name(mob_type_t, char *);
void mob_adjective(mob_type_t, char *);

#endif
