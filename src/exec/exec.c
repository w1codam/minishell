/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jde-groo <jde-groo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 14:53:22 by jde-groo      #+#    #+#                 */
/*   Updated: 2022/11/24 14:48:31 by buiterma      ########   odam.nl         */
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
	return (error("minishell", g_shell.cmds[index].args[0], \
			"command not found", 127));
}

void	ft_exec(size_t index)
{
	char	**norm_env;

	if (!g_shell.cmds[index].args || \
		g_shell.cmds[index].fd_in < 0 || g_shell.cmds[index].fd_out < 0)
		exit(g_shell.cmds[index].fd_in < 0 && g_shell.cmds[index].fd_out);
	if (g_shell.cmds[index].fd_in != STDIN_FILENO)
	{
		dup2(g_shell.cmds[index].fd_in, STDIN_FILENO);
		close(g_shell.cmds[index].fd_in);
	}
	if (g_shell.cmds[index].fd_out != STDOUT_FILENO)
	{
		dup2(g_shell.cmds[index].fd_out, STDOUT_FILENO);
		close(g_shell.cmds[index].fd_out);
	}
	if (g_shell.cmds[index].path == NULL && !g_shell.cmds[index].invalid)
		exit(exec_builtin(index));
	norm_env = normalize_env();
	execve(g_shell.cmds[index].path, g_shell.cmds[index].args, norm_env);
	ft_freearray(norm_env);
	if (access(g_shell.cmds[index].args[0], R_OK) == -1 || \
		(ft_strncmp("..", g_shell.cmds[index].args[0], 2) == 0))
		exit(error("minishell", g_shell.cmds[index].args[0], \
					"command not found", 127));
	exit(error(0, g_shell.cmds[index].args[0], "Permission denied", 126));
}

bool	exec_child(int index)
{
	int		fd[2];

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
		if ((size_t)index != g_shell.cmd_n - 1)
			dup2(fd[WRITE], STDOUT_FILENO);
		close(fd[WRITE]);
		close(fd[READ]);
		ft_exec(index);
	}
	else
	{
		dup2(fd[READ], STDIN_FILENO);
		close(fd[READ]);
		close(fd[WRITE]);
	}
	return (true);
}

static bool	single_builtin(void)
{
	if (ft_strcmp(g_shell.cmds[0].args[0], "echo") == 0)
		return (false);
	if (ft_strcmp(g_shell.cmds[0].args[0], "export") == 0 && \
		g_shell.cmds[0].args[1] == NULL)
		return (false);
	if (ft_strcmp(g_shell.cmds[0].args[0], "pwd") == 0)
		return (false);
	if (ft_strcmp(g_shell.cmds[0].args[0], "env") == 0)
		return (false);
	if (g_shell.cmds[0].fd_in < 0)
		g_shell.exit_code = 1;
	else
		g_shell.exit_code = exec_builtin(0);
	return (true);
}

bool	exec(void)
{
	int		status;

	if (g_shell.cmd_n == 0)
		return (true);
	if (g_shell.cmd_n == 1 && g_shell.cmds[0].path == NULL && \
		!g_shell.cmds[0].invalid && g_shell.cmds[0].args && single_builtin())
		return (true);
	if (!ft_fork(&g_shell.pid))
		return (false);
	if (g_shell.pid == 0 && !exec_func(0))
		return (false);
	sig_ignore();
	waitpid(g_shell.pid, &status, 0);
	if (WIFEXITED(status))
		g_shell.exit_code = WEXITSTATUS(status);
	return (true);
}
