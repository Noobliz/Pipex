/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:12:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/13 16:04:10 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_child(int i, int (*pipes)[2], t_cmd *cmds)
{
	int	input_fd;

	input_fd = open(cmds->file1, O_RDONLY);
	if (input_fd == -1)
	{
		free_all(pipes, cmds);
		perror("pipex infile");
		exit(EXIT_FAILURE);
	}
	if (dup2(input_fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(input_fd);
	if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		perror("dup2");
}

static void	last_child(int i, int (*pipes)[2], t_cmd *cmds)
{
	int	output_fd;

	output_fd = open(cmds->file2, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (!cmds->path)
	{
		error_message(cmds);
		free_all(pipes, cmds->head);
		if (output_fd > -1)
			close(output_fd);
		exit(127);
	}
	if (output_fd == -1)
	{
		perror("pipex outfile");
		free_all(pipes, cmds->head);
		exit(EXIT_FAILURE);
	}
	if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		perror("dup2");
	close(pipes[i - 1][0]);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		perror("dup2");
	close(output_fd);
}

static void	dup_and_exec(int i, int (*pipes)[2], t_cmd *current, char **envp)
{
	if (i == 0)
		first_child(i, pipes, current);
	else if (i == current->num_cmds - 1)
		last_child(i, pipes, current);
	else
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			perror("dup2 stdin");
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			perror("dup2 stdout");
	}
	close_pipes(pipes, current->num_cmds);
	execve(current->path, current->args, envp);
	perror("pipex");
	free_cmd_list(current->head);
	free(pipes);
	exit(126);
}

static void	handle_cmds(t_cmd *cmds, int (*pipes)[2], int fd2, char **envp)
{
	int		i;
	t_cmd	*current;
	pid_t	pids;

	i = 0;
	current = cmds;
	file2_rights(&fd2, current);
	while (current && current->num_cmds > 0)
	{
		create_kids(&pids, cmds, pipes);
		if (pids == 0)
		{
			if ((!current->path && current->next != NULL) || (fd2 == -42
					&& current->next == NULL))
			{
				error_message(current);
				free_all(pipes, cmds);
				exit(127);
			}
			dup_and_exec(i, pipes, current, envp);
			current->num_cmds--;
		}
		current = current->next;
		i++;
	}
}

void	pipex(t_cmd *cmds, char **envp)
{
	int	fd2;

	int(*pipes)[2];
	fd2 = 0;
	pipes = malloc(sizeof(int[2]) * (cmds->num_cmds));
	create_pipes(cmds, pipes);
	handle_cmds(cmds, pipes, fd2, envp);
	close_pipes(pipes, cmds->num_cmds);
	wait_for_kids(cmds->num_cmds);
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
// 				ft_printf("pipex: %s: command not found\n",
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
