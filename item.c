#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "item.h"

void item_name(char * s, uint32_t item)
{
    switch (item & 0xFF)
    {
    case item_dildo:
        strcpy(s, "a purple dildo");
        break;

    case item_leather_pants:
        strcpy(s, "a pair of leather pants");
        break;

    case item_med_prot:
        strcpy(s, "a medallion of protection");
        break;

    default:
        strcpy(s, "a rift in time and space");
        break;
    }
}