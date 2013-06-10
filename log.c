#include <stdlib.h>
#include <string.h>

#include "log.h"

char log_input[MAX_INPUT_LENGTH];
char * message_log[LOG_SIZE];
int message_log_index = -1;

void reset_log()
{
    while(message_log_index >= 0)
    {
        free(message_log[message_log_index]);
        message_log_index -= 1;
    }
}

void add_log(char * message)
{
    char * log_message = malloc(sizeof(char) * strlen(message) + 1);
    memcpy(log_message, message, strlen(message) + 1);

    message_log[++message_log_index] = log_message;
}
