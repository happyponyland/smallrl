#include "standard_dungeon_internal_item_tables.h"

#include <stdlib.h>
#include <string.h>

#define l_1_size (2)

static item_table_entry_t l_1_items[l_1_size] = {
    {
        .chance_ratio = 500,
        .item = {
            .type = item_type_armor,
            .subtype = 0,
        }
    },
    {
        .chance_ratio = 500,
        .item = {
            .type = item_type_potion,
            .subtype = item_subtype_potion_heal,
        }
    }
};

static item_t * copy_item(item_t *);

item_t * create_random_item(int depth)
{
    item_table_entry_t * items;
    int item_count;

    items = NULL;
    item_count = 0;

    switch((depth / 2) + rand() % (depth + 2))
    {
        case 0:
        case 1:
        case 2:
            items = l_1_items;
            item_count = l_1_size;
            break;
        case 3:
        case 4:
        case 5:
            items = l_1_items;
            item_count = l_1_size;
            break;
        default:
            break;
    }

    int total_ratio = 0;
    for(int i = 0; i < item_count; i += 1)
    {
        total_ratio += items[i].chance_ratio;
    }

    int random = rand() % total_ratio;

    int accumulator = 0;
    for(int i = 0; i < item_count; i += 1)
    {
        accumulator += items[i].chance_ratio;

        if(random <= accumulator) {
            return copy_item(&items[i].item);
        }
    }

    return NULL;
}

static item_t * copy_item(item_t * original) {
    item_t * new_item = malloc(sizeof(item_t));

    memcpy(new_item, original, sizeof(item_t));

    return new_item;
}
