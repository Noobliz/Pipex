/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:12:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/04 13:14:49 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child(int i, int (*pipes)[2], t_cmd *cmds)
{
	int	input_fd;

	input_fd = open(cmds->data->file1, O_RDONLY);
	if (input_fd == -1)
	{
		perror("open error");
		exit(EXIT_FAILURE);
	}
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
	dup2(pipes[i][1], STDOUT_FILENO);
}
void	last_child(int i, int (*pipes)[2], t_cmd *cmds)
{
	int	output_fd;

	output_fd = open(cmds->data->file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
	{
		perror("open error");
		exit(EXIT_FAILURE);
	}
	dup2(pipes[i - 1][0], STDIN_FILENO);
	close(pipes[i - 1][0]);
	dup2(output_fd, STDOUT_FILENO);
}

void	dup_and_exec(int i, int (*pipes)[2], t_cmd current, char **envp)
{
	if (i == 0)
		first_child(i, pipes, &current);
	else if (i == current.num_cmds - 1)
		last_child(i, pipes, &current);
	else
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	close_pipes(pipes, current.num_cmds);
	execve(current.path, current.args, envp);
	perror("execve error");
	exit(EXIT_FAILURE);
}
void	execute_commands(t_cmd *cmds, char **envp)
{
	pid_t	pids;
	t_cmd	*current;
	int		i;

	int(*pipes)[2];
	pipes = malloc(sizeof(int[2]) * (cmds->num_cmds));
	current = cmds;
	create_pipes(cmds->num_cmds, pipes);
	i = 0;
	while (current)
	{
		// create_kids(i, pids);
		pids = fork();
		if (pids == 0)
			dup_and_exec(i, pipes, *current, envp);
		current = current->next;
		i++;
	}
	close_pipes(pipes, cmds->num_cmds);
	// wait_for_kids(cmds->num_cmds, pids);
	while (i > 0)
	{
		wait(NULL);
		i--;
	}
	free(pipes);
}
