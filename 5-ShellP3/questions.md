1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation ensures that all child processes complete before the shell continues accepting user input by using waitpid() in my execute_pipeline() function. The purpose of this is to make sure the shell doesn't move on until all the commands have executed. If I forgot to call waitpid() on all child processes, the shell would just continue exectuing while the processes ae running which could lead to errors.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() because the processes need to be properly terminated to prevent any errors. If the pipes are left open, processes waiting for additional input would never receive an EOF signal. It could also lead to new processes not being able to be created.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is implemented as a built-in rather than an external command because the shell proccesses are affected when the user changes directories. If cd was implemented as an external process, the shell would stay in the same directory after the command is sent. Having it built-in makes sure that chdir() changes the working directory.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I would modify it by using dynamic memory allocation instead. The current implementation uses command_list_t with a commands[CMD_MAX], but this could be changed to cmd_buff_t\*commands which is allocating dynamically with realloc(). One trade-off to consider here is it's harder to maintain due to possible leaks if not freed properly. Another one is the performance of the program since it needs to reallocate many times compared to a fixed buffer.
