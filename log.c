#include <stdlib.h>
#include <string.h>

#include "log.h"

void init_log(log_t * log)
{
    log->message_log_index = -1;
}

void reset_log(log_t * log)
{
    while(log->message_log_index >= 0)
    {
        free(log->message_log[log->message_log_index]);
        log->message_log_index -= 1;
    }
}

void add_log(log_t * log, char * message)
{
    char * log_message = malloc(sizeof(char) * strlen(message) + 1);
    memcpy(log_message, message, strlen(message) + 1);

    log->message_log[++log->message_log_index] = log_message;
}
