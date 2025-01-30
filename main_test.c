/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:50:58 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/30 16:19:23 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child(int pid1, int input_fd, int *pipefd, t_data *data)
{
	if (pid1 == 0)
	{
		data->cmd_opt = get_cmd(data->argv[2]);
		data->cmd = find_path(data->env, data->cmd_opt[0]);
		if (data->cmd == NULL)
		{
			close(input_fd);
			free_path(data->cmd_opt);
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);             // fermer la lecture dans fils de cm1
		dup2(input_fd, STDIN_FILENO); // redirige stdin vers file1
		close(input_fd);              // car il a été dupliqué
		// rediriger stdout vers le pipe
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		// executer cmd1
		execve(data->cmd, data->cmd_opt, NULL);
		perror("execve cmd1");
		exit(1);
	}
}
int	main(int argc, char **argv, char **envp)
{
	int pipefd[2];
	pid_t pid1;
	pid_t pid2;
	int input_fd;
	int output_fd;
	char *env;

	check_files(argc, argv);
	env = get_env(envp);
	if (!env)
	{
		ft_printf("error\n");
		return (0);
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	input_fd = open(argv[1], O_RDONLY);
	// create fork for cmd1
	if ((pid1 = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}
	// fils cmd1
	if (pid1 == 0)
	{
		t_data data;
		data.cmd_opt = get_cmd(argv[2]);
		data.cmd = find_path(env, data.cmd_opt[0]);
		if (data.cmd == NULL)
		{
			close(input_fd);
			free_path(data.cmd_opt);
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);             // fermer la lecture dans fils de cm1
		dup2(input_fd, STDIN_FILENO); // redirige stdin vers file1
		close(input_fd);              // car il a été dupliqué
		// rediriger stdout vers le pipe
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		// executer cmd1
		execve(data.cmd, data.cmd_opt, NULL);
		perror("execve cmd1");
		exit(1);
	}
	if ((pid2 = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}
	// fils cmd2
	output_fd = open(argv[argc - 1], O_WRONLY);
	if (pid2 == 0)
	{
		t_data data;
		data.cmd_opt = get_cmd(argv[3]);
		data.cmd = find_path(env, data.cmd_opt[0]);
		if (data.cmd == NULL)
		{
			close(output_fd);
			free_path(data.cmd_opt);
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]); // ferme l'écriture
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
		// fait pointer entrée stand vers pipe de lecture
		if (execve(data.cmd, data.cmd_opt, NULL) == -1)
			perror("execve cmd2");
		exit(1);
	}
	// Processus père
	// Fermer les descripteurs du pipe dans le père
	close(pipefd[0]);
	close(pipefd[1]);
	// Attendre la fin des deux processus fils
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}