1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

   > **Answer**: We use fork/execvp instead of just calling execvp directly because it helps with concurrency and error handling. Fork allows for the creation of multiple child processes which means the parent can continue its execution while the child executes a new command. Like the parent, the child process matains the original memory allocations, file descriptors, etc.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

   > **Answer**: If the fork() system call fails, it returns a -1 to the parent process which shows there's an error. My implementation handles this scenario by calling perror("fork failed") followed by an ERR_EXEC_CMD to indicate the command couldn't be executed.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

   > **Answer**: execvp() finds the command to execute by looking through the directories listed in the PATH environment variable which plays a role in this process. What this environment variable does is store a list of directories where exe files are found. When a command is inputted, the directories are checked to find the exe file.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

   > **Answer**: The purpose of calling wait() in the parent process after forking is to make sure both parent and child processes are complete. If wait() isn't called, the parent process might terminate first and won't receive the exit status of the child process.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

   > **Answer**: WEXITSTATUS() provides the exit status code taken from the child process when wait() is called. It's important because it allows the parent process to get this exit status code. It also helps with error handling as it lets us know if there are errors with the execution.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

   > **Answer**: My implementation of build_cmd_buff() handles quoted arguments by detecting if the word starts and ends with a quotation mark. It also checks for leading whitespaces and skips those. If it's past the limit, an error ERR_TOO_MANY_COMMANDS will be returned. This is necessary because it lets us deal with multiple word arguments with spaces. Overall, it allows for correct parsing prevents unexpected bugs.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

   > **Answer**: This time around I was able to implement it into build_cmd_buff() easier. I was able to handle whitespaces, quoted strings, unquoted strings, and additional error handling. I didn't have any unexpected challenges for this one.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

  > **Answer**: The purpose of signals in a Linux system is to notify an ongoing process of anything new happening such as termination, user input, etc. They differ from other forms because signals can interrupt a process at any time and can better communicate between the different processes.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

  > **Answer**: SIGKILL is a signal that terminates a process and it's typical use case is to immediately terminate a process if it's unresponsive. It's usually a last resort if SIGTERM doesn't work. SIGTERM is like SIGKILL, but the difference is it performs cleanup before shutting down. The typical use case is to terminate a program safely. SIGINT is used to stop an ongoing process and is associated with the CTRL+C shortcut. The typical use case is using it to stop ongoing processes like scripts.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

  > **Answer**: WHen a process receives SIGSTOP, it gets suspended and the process stops executing. It can't be caught or ignored because it's a way for the OS to pause a process without interruption.
