1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

   > **Answer**: fgets() is a good choice for this application because it reads a line of user input while preventing buffer overflows. It also detects newline characters which makes it easier to detect when an input has ended. This is crucial since we need to process line by line.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

   > **Answer**: malloc() is needed to allocate memory for cmd_buff because size of input depends on user's input. This ensures it can handle the allocation of memory based on size of input even if it was large.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

   > **Answer**: Trimming leading and trailing spaces from each command before storing it is necessary because it makes sure the command is parsed correctly. If they weren't trimmed, arguements might not be detected properly which can lead to error where it's not detected as a command.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

  > **Answer**: One example that we should implement is redirecting stdout to an external file. Any command in the terminal would be sent to another file that handles all the commands. One challenge with this is the handling of file creation and opening is correct. The file descriptor also needs to be correctly redirected or errors will occur. ANother example is redirecting stderr to an external file. Any error messages would be sent to another file that handles all the error logs. One challenge with this is making sure file descriptors are redirectly properly and stderr is correctly handled. One last example is combining both stdout and stderr to an external file. Both output and error logs would be sent to same file. For example, it could be named output.log where both of them is sent. A challenge is redirecting them both correctly and at the same time.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

  > **Answer**: The difference between redirecting and piping is redirection is used to change input/output destinations while piping is used to connect outputs to an input from another command. An example of redirection is using it to control where input/output is redirected to like an external file. An example of piping is using it for linking two commands.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

  > **Answer**: It's important to keep these two separate because it keeps the code clean. It allows users to know which ones are errors and outputs rather than getting them mixed up which improves debugging times. It also allows for redirection and piping to be implemented easier. For example, stdout is passed to the next command while piping, but stderr is usually not. This prevents error messages messing with the pipline.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

  > **Answer**: When a command fails, the custom shell should handle it by sending errors to stderr and outputs to stdout. It's crucial to keep these two separate to know which ones which like previously stated in the last question. Our custom shell should also get the exit status of the commands which would let the user know if their command worked or not. We should provide a way to merge them because there will be times where users would want to merge the two. In the case of troubleshooting, it could be wise to send them both to the same file like I mentioned as one redirection example. This can be done by using an operator that combines them both before having them redirected. For example, 2>&1 can be used to merge both while just > can be used for stdout.
