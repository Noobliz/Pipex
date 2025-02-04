/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:23:21 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/03 15:59:17 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipes(int num_cmds, int (*pipes)[2])
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
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
	while (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
void	create_kids(int i, pid_t *pids)
{
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork error");
		exit(EXIT_FAILURE);
	}
}

void	wait_for_kids(int num_cmds, pid_t *pids)
{
	int i;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}