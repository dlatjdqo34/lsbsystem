#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

#include <utils.h>
#include <input.h>

#define SHELL_TOKEN_NUM     64
#define SHELL_TOKEN_DELIM   " \t\n\r\a"
#define BUILTIN_CMD_NUM     sizeof(builtin_shell_commands) / sizeof(map_cmd)

/* Sleep time after thread creates */
static unsigned int input_sleep_second = 2;


int cmd_exit(char **args);

/* [Begin] Data structure for mapping between command and function */

typedef struct 
{
    char *cmd;
    int (*func)(char **);
} map_cmd;

map_cmd builtin_shell_commands[] = {
    { "exit", &cmd_exit },
}; /* [End] Data structure for mapping between command and function */

/* [Begin] Commands for shell */

int cmd_exit(char **args)
{
    printf("[SBSH]\t Terminate Program...\n");

    /* Terminate this process */
    exit(EXIT_SUCCESS);

    /* UNREACHED */
    return 0;
} /* [End] Commands for shell */


/* [Begin] Shell thread related functions */

char *shell_read_line()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    // Get input command
    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            err_exit("getline in shell_parse_line()");
        }
    }

    assert(line != NULL);
    return line;
}


char **shell_parse_line(char *line)
{
    /* Defualt number of parsed token ptr */
    int tok_num = SHELL_TOKEN_NUM, pos = 0;
    char *token, **saved_token;
    char **tokens = malloc(tok_num * sizeof(char *));

    if (!tokens) {
        err_exit("malloc in shell_parse_line()");
    }

    token = strtok(line, SHELL_TOKEN_DELIM);
    while (token) {
        tokens[pos] = token;
        pos++;

        if (pos >= tok_num) {
            tok_num += SHELL_TOKEN_NUM;
            saved_token = tokens;
            tokens = realloc(tokens, tok_num * sizeof(char *));
            if (!tokens) {  
                free(saved_token);
                err_exit("realloc in shell_parse_line()");
            } /* realloc failed */
        }
        token = strtok(NULL, SHELL_TOKEN_DELIM);
    }

    tokens[pos] = NULL;
    return tokens;
}

int shell_execute(char **args)
{
    char *cmd = args[0];
    int i, status = 0;

    if (cmd == NULL) 
        return 0;

    for (i = 0; i < BUILTIN_CMD_NUM; i++) {
        if (!strcmp(cmd, builtin_shell_commands[i].cmd)) {
            status = builtin_shell_commands[i].func(args);
            break;
        }
    }

    return status;
}

void shell_loop()
{
    char *line;
    char **args;
    int status;

    do {
        printf("sbsh> ");
        line = shell_read_line(); 
        args = shell_parse_line(line);
        status = shell_execute(args);
        free(line);
        free(args);
    } while (status >= 0);
}

void *shell_thread_fn(void *arg)
{
    char *thread_name = (char *)arg;

    printf("[%s]\t thread created! Shell starts in %d seconds...\n", 
            thread_name, input_sleep_second);

    sleep(input_sleep_second);
    shell_loop();

    return NULL;
} /* [End] Shell thread related functions */


/* [Begin] Shell thread related functions */

int input_process()
{
    const char *name = "INPUT";
    int tid;
    void *status;
    pthread_t shell_thread;
    // pthread_attr_t attr;
    
    if (prctl(PR_SET_NAME, (unsigned long) name) < 0)
        perror("[%s]\t Process nanme unchanged... Keep going\n");
    printf("\n[%s]\t %s process created!\n\n", name, name);

    /* Create shell thread for built-in command */
    tid = pthread_create(&shell_thread, NULL, shell_thread_fn, "SBSH");
    assert(tid==0);

    pthread_join(shell_thread, &status);

    return 0;
}

pid_t create_input() 
{
    pid_t pid;
    
    printf("...\t Create input process...\n");
    switch ((pid = fork()))
    {
    case -1:
        err_exit("INPUT fork error...");
        break;
    case 0: /* Child Process */
        input_process();
        _exit(EXIT_SUCCESS);
        break;
    default: /* Back to Main Process */
        break;
    }

    return pid;
} /* [End] Shell thread related functions */