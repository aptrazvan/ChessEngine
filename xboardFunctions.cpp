#include "definitions.h"

void removeSigInt()
{
	char input[STR_BUFF];
	
	while (scanf("%s", input))
    {
        if (input[0] == 'p')
        {
           printf("feature sigint=0 sigterm=0 san=0\n");
           fflush(stdout);

           break;
        }
    }
}
