#ifndef RELOADER_H_
    #define RELOADER_H_

    #include <stdbool.h>
    #include <sys/types.h>

enum {
    FORK_IS_CHILD = 0,
    FORK_FAILURE = -1
};

typedef enum {
    REL_RUNNER,
    REL_RELAYER,
    REL_END,
    REL_ERRORED,
} relayer_state_t;

typedef struct {
    pid_t twin_pid;
    relayer_state_t role;
} reloader_t;

reloader_t *reloader_init(void);
bool reloader_manage(reloader_t *rel);

#endif /* !RELOADER_H_ */
