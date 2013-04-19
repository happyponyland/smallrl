#include "chaos_internal_ai.h"

void chaos_ai_select_spell(wizard_t * wizard)
{
    wizard->selected_spell = &wizard->spells[0];

    return;
}
