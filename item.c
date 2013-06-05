#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "item.h"

void item_name(char * s, item_t * item)
{
    switch (item->type)
    {
    case item_type_armor:
        strcpy(s, "a piece of armour");
        break;

    case item_type_potion:
        strcpy(s, "a potion");
        break;

    case item_type_garnish:
        strcpy(s, "a garnish");
        break;

    case item_type_bibelot:
        strcpy(s, "a bibelot");
        break;

    case item_type_key:
        strcpy(s, "a key");
        break;

    default:
        strcpy(s, "a rift in time and space");
        break;
    }
}
