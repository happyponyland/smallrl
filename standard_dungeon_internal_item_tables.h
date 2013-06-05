#ifndef GEN_STANDARD_DUNGEON_INTERNAL_ITEM_TABLES_H
#define GEN_STANDARD_DUNGEON_INTERNAL_ITEM_TABLES_H

#include "item.h"

typedef struct item_table_entry_s
{
    int chance_ratio;
    item_t item;
} item_table_entry_t;

item_t * create_random_item(int level);

#endif
