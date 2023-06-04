#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

int main(int argc DEBUG_USED, char **argv DEBUG_USED)
{
    printf("Hello!\n");
    DEBUG("Received %d arguments.", argc);
    return EXIT_SUCCESS;
}
