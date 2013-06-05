#ifndef SMALLRL_ITEM_H
#define SMALLRL_ITEM_H

#include <stdint.h>

typedef enum item_type_e
{
    item_type_void,
    item_type_armor,
    item_type_garnish,
    item_type_potion,
    item_type_bibelot,
    item_type_key, /* subtype = key shape */
    item_type_max /* index of last item + 1 */
} item_type_t;

typedef enum item_subtype_armor_e
{
    item_subtype_armor_cloth,
    item_subtype_armor_leather,
    item_subtype_armor_chainmail,
    item_subtype_armor_platemail
} item_subtype_armor_t;

typedef enum item_subtype_garnish_e
{
    item_subtype_garnish_necklace,
    item_subtype_garnish_ring,
    item_subtype_garnish_nosering
} item_subtype_garnish_t;

typedef enum item_subtype_potion_e
{
    item_subtype_potion_heal,
    item_subtype_potion_mana,
    item_subtype_potion_sleep,
    item_subtype_potion_stat_str
} item_subtype_potion_t;

typedef enum item_subtype_bibelot_e
{
    item_subtype_bibelot_magic_lamp,
    item_subtype_bibelot_dildo,
    item_subtype_bibelot_flute
} item_subtype_bibelot_t;

typedef struct item_s
{
    item_type_t type;
    uint32_t subtype;
    int state;
} item_t;

void item_name(char *, item_t *);

#endif
