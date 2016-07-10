# libESRI
A telnet framework library for applications

## Features
libESRI is a DLL with a C-Api that can be used to add a telnet client to an existing application.
The application can then be controlled using a telnet client.

- **C-Style Api**. Can be used with any langauge that supports calling C Functions from a DLL and function pointers
- **Multiuser support**. Every TCP Connection represents a user, which has its own handler
- **Unsafe**. It's still Telnet without any authentication or identification of incoming connections.
- **No dependencies**. Compile the library once, use it from any application
