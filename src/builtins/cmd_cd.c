/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_cd.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 13:34:33 by buiterma      #+#    #+#                 */
/*   Updated: 2022/11/10 13:49:43 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static bool	set_pwd(char *old_dir)
{
	char	cur_dir[PATH_MAX];
	char	*dup_cur;
	char	*dup_old;

	getcwd(cur_dir, PATH_MAX);
	dup_cur = ft_strdup(cur_dir);
	if (!set_env("PWD", dup_cur))
		return (FALSE);
	dup_old = ft_strdup(old_dir);
	if (!set_env("OLDPWD", dup_old))
	{
		free (dup_cur);
		return (FALSE);
	}
	return (TRUE);
}

static bool	set_dir(const char *path)
{
	char	cur_dir[PATH_MAX];

	getcwd(cur_dir, PATH_MAX);
	if (!path || chdir(path) < 0)
		return (error((char *)path, "No such file or directory", NULL, 0));
	if (!set_pwd(cur_dir))
		return (FALSE);
	return (TRUE);
}

static int	flags(int argc, const char *path)
{
	char	*tmp;
	t_env	*oldpwd;

	if ((ft_strncmp(path, "~", 1) == 0 && ft_strlen(path) == 1) || argc == 1)
	{
		tmp = getenv("HOME");
		if (!tmp)
			return (EXIT_FAILURE);
		if (!set_dir(tmp))
			return (EXIT_FAILURE);
	}
	else if (ft_strncmp(path, "-", 1) == 0 && ft_strlen(path) == 1)
	{
		oldpwd = get_env(g_shell.env, "OLDPWD");
		if (!oldpwd)
			return (error("minishell", "cd", "OLDPWD not set", 1));
		ft_putendl_fd(oldpwd->value, 1);
		if (set_dir(oldpwd->value))
			return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}

int	cmd_cd(int argc, const char **argv)
{
	t_env	*tmp;

	if (ft_strlen(argv[1]) == 1)
		return (flags(argc, argv[1]));
	if (argc == 1)
	{
		tmp = get_env(g_shell.env, "HOME");
		if (!tmp)
			return (error("minishell", "cd", "HOME not set", 1));
		if (set_dir(tmp->value))
			return (EXIT_SUCCESS);
	}
	else if (ft_strncmp(argv[1], "-nuts", 5) == 0)
		ft_putstr_fd("CDEEZ NUTZ! :O\n", 1);
	else if (!set_dir(argv[1]))
		return (EXIT_FAILURE);
	return (EXIT_FAILURE);
}
