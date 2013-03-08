#ifndef SMALLRL_CHAOS_INTERNAL_STARTPOSITIONS_H
#define SMALLRL_CHAOS_INTERNAL_STARTPOSITIONS_H

#include "chaos_internal.h"

typedef enum start_position_e
{
    start_position_upper_left,
    start_position_upper_center,
    start_position_upper_right,
    start_position_middle_left,
    start_position_middle_center,
    start_position_middle_right,
    start_position_lower_left,
    start_position_lower_center,
    start_position_lower_right
} start_position_t;

start_position_t wizard_start_positions_2[2] = { start_position_middle_left, start_position_middle_right };
start_position_t wizard_start_positions_3[3] = { start_position_upper_center, start_position_middle_left, start_position_middle_right };
start_position_t wizard_start_positions_4[4] = { start_position_upper_left, start_position_upper_right, start_position_lower_left, start_position_lower_right };
start_position_t wizard_start_positions_5[5] = { start_position_upper_left, start_position_upper_right, start_position_lower_left, start_position_lower_right, start_position_middle_center };
start_position_t wizard_start_positions_6[6] = { start_position_upper_left, start_position_upper_center, start_position_upper_right, start_position_lower_left, start_position_lower_center, start_position_lower_right };
start_position_t wizard_start_positions_7[7] = { start_position_upper_left, start_position_upper_center, start_position_upper_right, start_position_lower_left, start_position_lower_center, start_position_lower_right, start_position_middle_center };
start_position_t wizard_start_positions_8[8] = { start_position_upper_left, start_position_upper_center, start_position_upper_right, start_position_middle_left, start_position_middle_right, start_position_lower_left, start_position_lower_center, start_position_lower_right };
start_position_t wizard_start_positions_9[9] = { start_position_upper_left, start_position_upper_center, start_position_upper_right, start_position_middle_left, start_position_middle_center, start_position_middle_right, start_position_lower_left, start_position_lower_center, start_position_lower_right };

#endif
