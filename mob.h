#ifndef SMALLRL_MOB_H
#define SMALLRL_MOB_H

typedef enum mob_type_e
{
    mob_none = 0,
    mob_player = '@',
    mob_newbie = 'n',
    mob_zombie = 'z'
} mob_type_t;

typedef struct mob_s
{
    mob_type_t type;
    int y;
    int x;
    int hp;
    int turn_counter;
} mob_t;

void mob_name(char *, mob_type_t);

#endif
