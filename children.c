/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:12:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/11 13:39:46 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(int (*pipes)[2], t_cmd *cmds)
{
	close_pipes(pipes, cmds->num_cmds);
	free_cmd_list(cmds);
	free(pipes);
}

void	first_child(int i, int (*pipes)[2], t_cmd *cmds)
{
	int	input_fd;

	input_fd = open(cmds->file1, O_RDONLY);
	if (input_fd == -1)
	{
		free_all(pipes, cmds);
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

	output_fd = open(cmds->file2, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (output_fd == -1)
	{
		perror("open error");
		exit(EXIT_FAILURE);
	}
	dup2(pipes[i - 1][0], STDIN_FILENO);
	close(pipes[i - 1][0]);
	dup2(output_fd, STDOUT_FILENO);
}

void	dup_and_exec(int i, int (*pipes)[2], t_cmd *current, char **envp)
{
	if (i == 0)
		first_child(i, pipes, current);
	else if (i == current->num_cmds - 1)
		last_child(i, pipes, current);
	else
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	close_pipes(pipes, current->num_cmds);
	execve(current->path, current->args, envp);
	perror("pipex");
	exit(126);
}
void	file2_rights(int *fd2, t_cmd *cmds)
{
	*fd2 = open(cmds->file2, O_WRONLY);
	if (*fd2 == -1)
	{
		if (access(cmds->file2, F_OK) == 0)
		{
			cmds->num_cmds--;
			*fd2 = -42;
		}
	}
	else
		close(*fd2);
}

void	execute_commands(t_cmd *cmds, char **envp)
{
	pid_t	pids;
	t_cmd	*current;
	int		i;
	int		fd2;

	fd2 = 0;
	file2_rights(&fd2, cmds);
	int(*pipes)[2];
	pipes = ft_calloc(sizeof(int[2]), (cmds->num_cmds));
	current = cmds;
	create_pipes(cmds->num_cmds, pipes);
	i = 0;
	while (current && current->num_cmds > 0)
	{
		create_kids(&pids, cmds, pipes);
		if (pids == 0)
		{
			if (!current->path || (fd2 == -42 && current->next == NULL))
			{
				if (!current->path)
					ft_printf("pipex: command not found\n");
				else
					ft_printf("pipex : permission denied\n");
				free_all(pipes, cmds);
				exit(127);
			}
			dup_and_exec(i, pipes, current, envp);
			current->num_cmds--;
		}
		current = current->next;
		i++;
	}
	// wait_for_kids(cmds->num_cmds, pids);
	close_pipes(pipes, cmds->num_cmds);
	while (i > 0)
	{
		wait(NULL);
		i--;
	}
	free_cmd_list(cmds);
	free(pipes);
}
// NEW VERSION OU PAS
// void	execute_commands(t_cmd *cmds, char **envp)
// {
// 	int		i;
// 	pid_t	pids[cmds->num_cmds];
// 	int		pipes[cmds->num_cmds - 1][2];
// 	int		status;
// 	t_cmd	*current;

// 	i = 0;
// 	current = cmds;
// 	while (i < cmds->num_cmds - 1)
// 	{
// 		if (pipe(pipes[i]) == -1)
// 			error_exit("pipe", EXIT_FAILURE);
// 		i++;
// 	}
// 	i = 0;
// 	while (current)
// 	{
// 		pids[i] = fork();
// 		if (pids[i] == -1)
// 			error_exit("fork", EXIT_FAILURE);
// 		if (pids[i] == 0)
// 		{
// 			// SI CMD INTROUVABLE CODE ERREUR 127
// 			if (!current->path)
// 			{
// 				fprintf(stderr, "pipex: %s: command not found\n",
// 					current->args[0]);
// 				exit(127);
// 			}
// 			dup_and_exec(i, pipes, current, envp);
// 		}
// 		current = current->next;
// 		i++;
// 	}
// 	i = 0;
// 	while (i < cmds->num_cmds - 1)
// 	{
// 		close(pipes[i][0]);
// 		close(pipes[i][1]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < cmds->num_cmds)
// 	{
// 		waitpid(pids[i], &status, 0);
// 		if (WIFEXITED(status))
// 			exit(WEXITSTATUS(status));
// 		i++;
// 	}
// }
