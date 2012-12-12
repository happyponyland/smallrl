#ifndef SMALLRL_ITEM_H
#define SMALLRL_ITEM_H

#include <stdint.h>

#define ITEM_TYPE(x) ((x) & 0xFF)

typedef enum
{
    item_void,
    item_dildo,
    item_leather_pants,
    item_med_prot,
    item_healing_pot
} item_number_t;

void item_name(char *, uint32_t);

#endif
