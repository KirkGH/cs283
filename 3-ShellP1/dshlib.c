#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'. It then traverses each
 *  command. For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (!cmd_line || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    memset(clist, 0, sizeof(command_list_t));
    int cmd_index = 0;
    char *cmd_element = strtok(cmd_line, PIPE_STRING);

    while (cmd_element != NULL)
    {
        if (cmd_index >= CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }

        if (strlen(cmd_element) >= EXE_MAX)
        {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        if (strlen(cmd_element) == 0)
        {
            cmd_element = strtok(NULL, PIPE_STRING);
            continue;
        }

        while (*cmd_element == SPACE_CHAR)
        {
            cmd_element++;
        }

        char *arg_start = strchr(cmd_element, SPACE_CHAR);

        if (arg_start != NULL)
        {
            int exe_size = arg_start - cmd_element;
            if (exe_size >= EXE_MAX)
            {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            strncpy(clist->commands[cmd_index].exe, cmd_element, exe_size);
            clist->commands[cmd_index].exe[exe_size] = '\0';

            arg_start++;
            while (*arg_start == SPACE_CHAR)
            {
                arg_start++;
            }

            strncpy(clist->commands[cmd_index].args, arg_start, ARG_MAX - 1);
            clist->commands[cmd_index].args[ARG_MAX - 1] = '\0';
        } else {
            strncpy(clist->commands[cmd_index].exe, cmd_element, EXE_MAX - 1);
            clist->commands[cmd_index].exe[EXE_MAX - 1] = '\0';
            clist->commands[cmd_index].args[0] = '\0';
        }

        cmd_index++;
        cmd_element = strtok(NULL, PIPE_STRING);
    }

    clist->num = cmd_index;
    return OK;
}