#ifndef SMALLRL_LOG_H
#define SMALLRL_LOG_H

#define LOG_SIZE (1000)
#define MAX_INPUT_LENGTH (1000)

typedef struct log_s log_t;
struct log_s
{
    char log_input[MAX_INPUT_LENGTH];
    char * message_log[LOG_SIZE];
    int message_log_index;
};

void init_log(log_t *);
void reset_log(log_t *);
void add_log(log_t *, char *);

#endif
