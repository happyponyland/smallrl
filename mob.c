#include <string.h>

#include "mob.h"

void mob_name(char * s, mob_type_t type)
{
	switch (type)
	{
	case mob_player:
		strcpy(s, "player");
		break;

	case mob_newbie:
		strcpy(s, "newbie");
		break;

	default:
		strcpy(s, "???");
		break;
	}

	return;
}
