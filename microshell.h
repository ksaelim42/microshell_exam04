#ifndef MICROSHELL_H
# define MICROSHELL_H

#include <string.h> // strcmp
#include <unistd.h> // write, fork, execve, dup2
#include <sys/wait.h> // waitpid

// checks condition
# define IS_CD !strcmp(av[0], "cd")
# define IS_PIPE(i) !strcmp(av[i], "|")
# define IS_SEMIC(i) !strcmp(av[i], ";")
# define NOT_PIPE_SEMIC(i) !IS_PIPE(i) && !IS_SEMIC(i)

// close file
# define CFD(i) close(fd[i])

// error message
# define CD_WRONG_ARG errl("error: cd: bad arguments")
# define CD_FAILED errl_par("error: cd: cannot change directory to", todir)
# define EXEC_CD_ERROR errl("error: fatal")
# define EXEC_FAILED errl_par("error: cannot execute", av[0])

// print error function
void err(char *s);
int errl(char *s);
int errl_par(char *s1, char *s2);

#endif