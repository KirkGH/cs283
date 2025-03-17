1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines when a command's output is fully received from the server through predefined delimiters like EOF that tells the client the input has ended. One technique to handle partial reads is to loop through until this delimiter is detected. Another technique is a fixed length protocol where the server would send the message length to show the user how much data to expect.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol should define and detect the beginning and end of a command by using delimiters such as EOF characters which was discussed in the previous question. This would allow the message to be parsed easily. If it's not handled correctly, the remote client can incorrectly read incomplete commands. It may also lead to reading multiple commands at once.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols keeps the information about the state across multiple interactions. This means the server keeps track of previous requests such as connections. Stateless protocols don't keep this information and treats each request individually. The main difference between these two is stateful depends on previous requests while stateless treats them individually.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

One reason why we would use it is for the performance of it compared to TCP since it doesn't need as much processing. An example of where this would be beneficial is for video streaming since speed is crucial, but it can sacrifice reliability.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The socket API is provided by the operating system to enable applications to use network communications. These sockets allow programs to send or receive data over a network through protocols such as TCP or UDP.
