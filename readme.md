# MICROSHELL IN C

Microshell written purely in C.

## Compiling
Download
```
git clone https://github.com/vexinom/microshell.git
```

Debug verison
```
gcc -g -O0 microshell.c commands.c ls.c pwd.c echo.c cd.c cp.c -o microshell
./microshell
```

Realese version
```
gcc -O2 microshell.c commands.c ls.c pwd.c echo.c cd.c cp.c -o microshell
./microshell
```
