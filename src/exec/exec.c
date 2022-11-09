/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jde-groo <jde-groo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 14:53:22 by jde-groo      #+#    #+#                 */
/*   Updated: 2022/11/09 10:27:08 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	exec_builtin(int index)
{
	if (ft_strncmp("cd", g_shell.cmds[index].args[0], 3) == 0)
		return (cmd_cd(ft_arraylen(g_shell.cmds[0].args), \
		(const char **)g_shell.cmds[index].args));
	if (ft_strncmp("exit", g_shell.cmds[index].args[0], 5) == 0)
		return (cmd_exit(ft_arraylen(g_shell.cmds[index].args), \
		(const char **)g_shell.cmds[index].args));
	if (ft_strncmp("echo", g_shell.cmds[index].args[0], 5) == 0)
		return (cmd_echo(ft_arraylen(g_shell.cmds[index].args), \
		(const char **)g_shell.cmds[index].args));
	if (ft_strncmp("env", g_shell.cmds[index].args[0], 4) == 0)
		return (cmd_env(ft_arraylen(g_shell.cmds[index].args), \
		(const char **)g_shell.cmds[index].args));
	if (ft_strncmp("pwd", g_shell.cmds[index].args[0], 4) == 0)
		return (cmd_pwd());
	if (ft_strncmp("export", g_shell.cmds[index].args[0], 7) == 0)
		return (cmd_export(ft_arraylen(g_shell.cmds[index].args), \
		(const char **)g_shell.cmds[index].args));
	if (ft_strncmp("unset", g_shell.cmds[index].args[0], 6) == 0)
		return (cmd_unset(ft_arraylen(g_shell.cmds[index].args), \
		(const char **)g_shell.cmds[index].args));
	return (127);
}

static void	ft_execve(size_t index)
{
	if (index == 0 && g_shell.fd_in != STDIN_FILENO)
	{
		dup2(g_shell.fd_in, STDIN_FILENO);
		close(g_shell.fd_in);
	}
	if (index == g_shell.cmd_n - 1 && g_shell.fd_out != STDOUT_FILENO)
	{
		dup2(g_shell.fd_out, STDOUT_FILENO);
		close(g_shell.fd_out);
	}
	if (g_shell.cmds[index].path == NULL && !g_shell.cmds[index].invalid)
		exit(exec_builtin(index));
	execve(g_shell.cmds[index].path, g_shell.cmds[index].args, normalize_env());
	if (!access(g_shell.cmds[index].args[0], F_OK))
		exit(error("minishell", g_shell.cmds[index].args[0], 
			"Permission denied", 126));
	exit(error("command not found", g_shell.cmds[index].args[0], NULL, 127));
}

static bool	single_builtin(void)
{
	g_shell.exit_code = exec_builtin(0);
	return (true);
}

static pid_t	exec(size_t i, int fd_in)
{
	int		fd[2];

	set_sigs_exec();
	if (g_shell.cmd_n == 0)
		return (0);
	if (g_shell.cmd_n == 1 && g_shell.cmds[0].path == NULL && \
		!g_shell.cmds[0].invalid && single_builtin())
		return (0);
	if (!ft_pipe(fd))
		return (false);
	if (!ft_fork(&g_shell.pid))
	{
		close(fd[READ]);
		close(fd[WRITE]);
		return (false);
	}
	if (g_shell.pid == 0)
	{
		// dup2(fd[WRITE], STDOUT_FILENO);
		close(fd[WRITE]);
		close(fd[READ]);
		ft_execve(i - 1);
	}
	else
	{
		// dup2(fd[READ], STDIN_FILENO);
		close(fd_in);
		close(fd[WRITE]);
		if (i < g_shell.cmd_n)
			g_shell.pid = exec(i + 1, fd[READ]);
	}
	return (g_shell.pid);
}

	// if (!ft_pipe(fd))
	// 	return (0);
	// if (!ft_fork(&pid))
	// {
	// 	close(fd[READ]);
	// 	close(fd[WRITE]);
	// 	return (0);
	// }
	// if (pid == 0)
	// {
	// 	// dup2(fd[WRITE], STDOUT_FILENO);
	// 	close(fd[READ]);
	// 	ft_execve(i - 1);
	// }
	// // dup2(fd[READ], STDIN_FILENO);
	// close(fd_in);
	// close(fd[WRITE]);
	// if (i < g_shell.cmd_n)
	// 	pid = exec(i + 1, fd[READ]);
	// return (pid);
// }

bool	exec_init(void)
{
	int		fd;
	int		status;
	size_t	i;
	pid_t	pid;

	i = 0;
	fd = dup(STDIN_FILENO);
	if (fd < 0)
		return (false);
	pid = exec(0, fd);
	if (pid >= 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_shell.exit_code = WEXITSTATUS(status);
	}
	else
		g_shell.exit_code = 1;
	while (i < g_shell.cmd_n)
	{
		wait(NULL);
		i++;
	}
	return (true);
}
