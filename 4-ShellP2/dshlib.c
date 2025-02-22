#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
void print_dragon();

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (!cmd_buff->_cmd_buffer) return ERR_MEMORY;
    memset(cmd_buff, 0, sizeof(cmd_buff_t));
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
	    free(cmd_buff->_cmd_buffer);
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (!cmd_line || strlen(cmd_line) >= SH_CMD_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }

    cmd_buff->argc = 0;
    char *cursor = cmd_buff->_cmd_buffer;

    while (*cursor) {
        while (isspace((unsigned char)*cursor)) {
            cursor++;
        }

        if (*cursor == '\0') {
            break;
        }

        if (*cursor == '"') {
            cursor++;
            cmd_buff->argv[cmd_buff->argc++] = cursor;

            while (*cursor && *cursor != '"') {
                cursor++;
            }

            if (*cursor == '"') {
                *cursor = '\0';
                cursor++;
            }
        } else {
            cmd_buff->argv[cmd_buff->argc++] = cursor;

            while (*cursor && !isspace((unsigned char)*cursor)) {
                cursor++;
            }

            if (*cursor) {
                *cursor = '\0';
                cursor++;
            }
        }

        if (cmd_buff->argc >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "exit") == 0) return BI_CMD_EXIT;
    if (strcmp(input, "dragon") == 0) return BI_CMD_DRAGON;
    if (strcmp(input, "cd") == 0) return BI_CMD_CD;
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0) return BI_NOT_BI;

    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);
    switch (cmd_type) {
        case BI_CMD_EXIT:
            exit(OK_EXIT);
        case BI_CMD_DRAGON:
            print_dragon();
        case BI_CMD_CD:
            if (cmd->argc > 1) {
                chdir(cmd->argv[1]);
            }
            return BI_EXECUTED;
        default:
            return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd) {
    if (exec_built_in_cmd(cmd) == BI_EXECUTED) {
        return OK;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        exit(ERR_EXEC_CMD);
    } else if (pid > 0) {
        int statusCode;
        waitpid(pid, &statusCode, 0);
        if (WIFEXITED(statusCode)) {
		return WEXITSTATUS(statusCode); 
	    }
    } else {
        perror("error: fork failed");
        return ERR_EXEC_CMD;
    }
    return OK;
}

int exec_local_cmd_loop() {
    char *cmd_buff;
    cmd_buff_t cmd;
    
    if ((cmd_buff = malloc(SH_CMD_MAX)) == NULL || alloc_cmd_buff(&cmd) != OK) {
        free(cmd_buff);
        return ERR_MEMORY;
    }

    for (;;) {
        printf("%s", SH_PROMPT);

        if (!fgets(cmd_buff, SH_CMD_MAX, stdin)) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        int rc = build_cmd_buff(cmd_buff, &cmd);
        if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        if (exec_built_in_cmd(&cmd) == BI_NOT_BI) {
            exec_cmd(&cmd);
        }
    }

    free_cmd_buff(&cmd);
    free(cmd_buff);
    
    return OK;
}
