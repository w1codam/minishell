/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/28 12:53:14 by buiterma      #+#    #+#                 */
/*   Updated: 2022/11/10 16:56:29 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	sig_handler_exec(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		g_shell.exit_code = 130;
	}
	if (sig == SIGQUIT)
	{
		write(1, "Quit: 3\n", 8);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_shell.exit_code = 131;
	}
	exit(g_shell.exit_code);
}

static void	sighandler(int num)
{
	if (num == SIGINT)
	{	
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (num == SIGQUIT)
		return ;
}

void	init_signal(void)
{
	struct termios	t;
	extern int		rl_catch_signals;

	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);
	rl_catch_signals = false;
	signal(SIGQUIT, SIG_IGN);
	set_signals();
}

void	set_sigs_exec(void)
{
	struct sigaction	sig;

	sig.sa_handler = &sig_handler_exec;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
}

void	set_signals(void)
{
	struct sigaction	sig;

	sig.sa_handler = &sighandler;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
}
