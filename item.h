#ifndef SMALLRL_ITEM_H
#define SMALLRL_ITEM_H

#include <stdint.h>

typedef enum
{
    item_dildo = 1,
    item_leather_pants = 2,
    item_med_prot = 3
} item_number_t;

void item_name(char *, uint32_t);

#endif
