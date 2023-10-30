#include <string.h> // strcmp
#include <unistd.h> // write, fork, execve, dup2
#include <sys/wait.h> // waitpid

// checks condition
#define IS_CD !strcmp(av[0], "cd")
#define IS_PIPE(i) !strcmp(av[i], "|")
#define IS_SEMIC(i) !strcmp(av[i], ";")
#define NOT_PIPE_SEMIC(i) !IS_PIPE(i) && !IS_SEMIC(i)

// close file
#define CFD(i) close(fd[i])

// error message
#define CD_WRONG_ARG errl("error: cd: bad arguments")
#define CD_FAILED errl_par("error: cd: cannot change directory to", todir)
#define EXEC_CD_ERROR errl("error: fatal")
#define EXEC_FAILED errl_par("error: cannot execute", av[0])

void err(char *s)
{
	while (*s)
		write(2, s++, 1);
}

int errl(char *s)
{
	err(s);
	err("\n");
	return (1);
}

int errl_par(char *s1, char *s2)
{
	err(s1);
	err(" ");
	errl(s2);
	return (1);
}

int	cd(char *todir, int ncmd) {
	if (ncmd != 2)
		return CD_WRONG_ARG;
	else if (chdir(todir) == -1)
		return CD_FAILED;
	return 0;
}

int exec(char **av, char **envp, int ncmd)
{
	int fd[2];
	int status = 0;
	int ispipe = (av[ncmd] && IS_PIPE(ncmd));

	if (ispipe && pipe(fd) == -1)
		return EXEC_CD_ERROR;

	int pid = fork();
	if (!pid) {
		av[ncmd] = 0;
		if (ispipe && (dup2(fd[1], 1) == -1 || CFD(0) == -1 || CFD(1) == -1))
			return EXEC_CD_ERROR;
		execve(av[0], av, envp);
		return EXEC_FAILED;
	}
	waitpid(pid, &status, 0);
	if (ispipe && (dup2(fd[0], 0) == -1 || CFD(0) == -1 || CFD(1) == -1))
		return EXEC_CD_ERROR;
	return WIFEXITED(status) && WEXITSTATUS(status);
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	int status = 0;

	while (*av && *(av + 1)) {
		av++;
		int ncmd = 0;
		while (av[ncmd] && NOT_PIPE_SEMIC(ncmd))
			ncmd++;
		if (IS_CD)
			status = cd(av[1], ncmd);
		else if (ncmd)
			status = exec(av, envp, ncmd);
		av += ncmd;
	}
	return status;
}
