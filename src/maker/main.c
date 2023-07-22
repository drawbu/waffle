#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *get_run_cmd(char *target)
{
    char *result = malloc(22 + 4096 * 2);

    if (target == NULL || strcmp("waffle", target) == 0) {
        if (access("waffle", F_OK) != -1)
            (void) system("make");
        return "./waffle";
    }
    snprintf(result, 22 + 4096 * 2, "make clean; make %s; ./%s", target, target);
    return result;
}

int main(int argc, char **argv) 
{
    char *cmd = get_run_cmd(argc >= 2 ? argv[1] : NULL);
    int return_val;

    while (true) {
        return_val = system(cmd);
        if (return_val != EXIT_SUCCESS) {
            printf("Waffle exited with error code: %d.\nExiting...\n", return_val);
            free(cmd);
            break;
        }
        printf("Waffle exited with code %d.\nReloading...\n", return_val);
    }
    return EXIT_SUCCESS;
}
