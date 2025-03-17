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

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    memset(cmd_buff, 0, sizeof(cmd_buff_t));
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
    char input[SH_CMD_MAX];

    while (1) {
        printf("%s", SH_PROMPT);

        if (!fgets(input, SH_CMD_MAX, stdin)) {
            return OK;
        }

        input[strcspn(input, "\n")] = '\0';

        command_list_t clist;
        if (build_cmd_list(input, &clist) != OK) {
            continue;
        }

        if(clist.num > CMD_MAX){
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }

        if(clist.num == 0){
            continue;
        }
        
        if (clist.num == 1) {
            if (exec_built_in_cmd(&clist.commands[0]) == BI_CMD_EXIT) {
                return OK_EXIT;
            }
            exec_cmd(&clist.commands[0]);
        } else {
            execute_pipeline(&clist);
        }
    }
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *command;
    int commandCount = 0;
    clist->num = 0;
    memset(clist->commands, 0, sizeof(clist->commands));

    if (cmd_line == NULL || strlen(cmd_line) == 0 || strspn(cmd_line, " ") == strlen(cmd_line)) {
        return WARN_NO_CMDS;
    }

    command = strtok(cmd_line, PIPE_STRING);

    while (command != NULL) {
        char *endOfString = command + strlen(command) - 1;

        while (*command != '\0' && isspace(*command)) {
            command++;
        }

        while (endOfString > command && isspace(*endOfString)) {
            endOfString--;
        }
        *(endOfString + 1) = '\0';

        if (commandCount >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        if (alloc_cmd_buff(&clist->commands[commandCount])) {
            return ERR_MEMORY;
        }

        if (build_cmd_buff(command, &clist->commands[commandCount])) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        commandCount++;
        command = strtok(NULL, PIPE_STRING);
    }

    clist->num = commandCount;
    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int numOfCommands = clist->num;
    int prev_pipe_read = -1, pipefd[2];
    pid_t pids[numOfCommands];

    for (int i = 0; i < numOfCommands; i++) {
        if (i < numOfCommands - 1 && pipe(pipefd) == -1) {
            perror("pipe failed");
            return ERR_EXEC_CMD;
        }

        if ((pids[i] = fork()) == 0) {
            if (prev_pipe_read != -1) {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            if (i < numOfCommands - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                close(pipefd[0]);
            }
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            exit(ERR_EXEC_CMD);
        }

        if (pids[i] < 0) {
            return ERR_EXEC_CMD;
        }

        if (prev_pipe_read != -1) close(prev_pipe_read);
        if (i < numOfCommands - 1) {
            prev_pipe_read = pipefd[0];
            close(pipefd[1]);
        }
    }

    for (int i = 0; i < numOfCommands; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }

    return OK;
}