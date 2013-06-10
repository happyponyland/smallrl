#ifndef SMALLRL_LOG_H
#define SMALLRL_LOG_H

#define LOG_SIZE (1000)
#define MAX_INPUT_LENGTH (1000)

extern char log_input[MAX_INPUT_LENGTH];
extern char * message_log[LOG_SIZE];
extern int message_log_index;


void reset_log(void);
void add_log(char *);

#endif
