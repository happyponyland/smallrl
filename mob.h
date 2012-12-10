#ifndef SMALLRL_MOB_H
#define SMALLRL_MOB_H

#define ATTR_HP      0
#define ATTR_MINDAM  1
#define ATTR_MAXDAM  2
#define ATTR_ATTACK  3
#define ATTR_DODGE   4
#define ATTRIBUTES 15

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
    int attr[ATTRIBUTES];
    int turn_counter;
} mob_t;

void mob_name(char *, mob_type_t);
void mob_adjective(char *, mob_type_t);

#endif
