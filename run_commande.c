/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commande.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaili < yismaili@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 11:22:25 by souchen           #+#    #+#             */
/*   Updated: 2022/08/14 22:11:31 by yismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	run_commands(t_struct *shell)
{
	int	i;
	int	end[2];

	i = 0;
	shell->path = get_path(shell);
	while (i < shell->divise.pipe)
	{
		if (pipe(end) == -1)
			ft_die("pipe error\n");
		shell->output_fd = end[1];
		shell->last_in = end[0];
		shell->fd_out = end[1];
		if (next_run_commands(shell) == 1)
			return ;
		close(shell->output_fd);
		if (shell->input_fd != 0)
			close(shell->input_fd);
		shell->input_fd = end[0];
		i++;
	}
	run_commands_next(shell);
}

void	run_commands_next(t_struct *shell)
{
	shell->last_in = -1;
	if (next_run_commands(shell) == 1)
		return ;
	if (shell->input_fd != 0)
		close(shell->input_fd);
	if (shell->path)
		ft_free_cmd(shell->path);
	ft_wait_pid(shell);
	ft_cmd(shell->commands);
}

void	ft_wait_pid(t_struct *shell)
{
	int	j;

	j = 0;
	while (j <= shell->divise.pipe + 1)
	{
		wait(NULL);
		j++;
	}
}

int	next_run_commands(t_struct	*shell)
{
	if (fun_redirection(shell) == 0)
	{
		return (1);
	}
	if (shell->commands[0][0] != '>')
	{
		arguments_func(shell);
		if (execution(shell) == 1)
			return (1);
	}
	return (0);
}
