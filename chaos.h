#ifndef SMALLRL_CHAOS_H
#define SMALLRL_CHAOS_H

typedef enum chaos_outcome_e
{
    chaos_outcome_draw,
    chaos_outcome_loss,
    chaos_outcome_win
} chaos_outcome_t;

chaos_outcome_t chaos_duel(void);

#endif
