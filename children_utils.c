/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:23:21 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/12 12:55:14 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(int (*pipes)[2], t_cmd *cmds)
{
	close_pipes(pipes, cmds->num_cmds);
	free_cmd_list(cmds);
	free(pipes);
}

void	create_pipes(t_cmd *cmds, int (*pipes)[2])
{
	int	i;

	i = 0;
	while (i < cmds->num_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			free_all(pipes, cmds);
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
void	close_pipes(int (*pipes)[2], int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds -1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
void	create_kids(pid_t *pids, t_cmd *cmds, int (*pipes)[2])
{
	*pids = fork();
	if (*pids == -1)
	{
		free_all(pipes, cmds);
		perror("fork error");
		exit(EXIT_FAILURE);
	}
}

void	wait_for_kids(int num_cmds)
{
	int i;

	i = 0;
	while (i < num_cmds)
	{
		wait(NULL);
		i++;
	}
}