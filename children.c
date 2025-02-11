/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:12:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/11 17:22:25 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		// write(1, "hey\n", 4);
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			perror("dup2 stdin");
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			perror("dup2 stdout");
	}
	close_pipes(pipes, current->num_cmds);
	execve(current->path, current->args, envp);
	perror("pipex");
	exit(126);
}
void	error_message(t_cmd *current)
{
	if (!current->path)
		perror("cmd not found\n");
	else
		perror("pipex");
}
void	handle_cmds(t_cmd *cmds, int (*pipes)[2], int fd2, char **envp)
{
	int		i;
	t_cmd	*current;
	pid_t	pids;

	i = 0;
	current = cmds;
	while (current && current->num_cmds > 0)
	{
		create_kids(&pids, cmds, pipes);
		if (pids == 0)
		{
			if (!current->path || (fd2 == -42 && current->next == NULL))
			{
				error_message(current);
				    int j = 0;
    while (j < current->num_cmds - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        j++;
    }
				exit(127);
			}
			dup_and_exec(i, pipes, current, envp);
			current->num_cmds--;
		}
		current = current->next;
		i++;
	}
}

void	execute_commands(t_cmd *cmds, char **envp)
{
	int	fd2;

	int(*pipes)[2];
	fd2 = 0;
	file2_rights(&fd2, cmds);
	pipes = ft_calloc(sizeof(int[2]), (cmds->num_cmds));
	create_pipes(cmds, pipes);
	handle_cmds(cmds, pipes, fd2, envp);
	wait_for_kids(cmds->num_cmds);
	close_pipes(pipes, cmds->num_cmds);
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
