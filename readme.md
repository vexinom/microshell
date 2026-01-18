# MICROSHELL IN C

Microshell written purely in C.

## Compiling

Debug verison
```
gcc -g -O0 microshell.c commands.c ls.c pwd.c echo.c cd.c -o microshell
```

Realese version
```
gcc -O2 microshell.c commands.c ls.c pwd.c echo.c cd.c -o microshell
```
