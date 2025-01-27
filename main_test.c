/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:50:58 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/27 15:41:03 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	writestr(int fd, const char *str)
{
	write(fd, str, strlen(str));
}

int	main(void)
{
	int pipefd[2];
	pid_t pid;
	char buf;

	// creation du pipe
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	// creation du fils
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(pipefd[1]); // ferme bout d'ecriture car inutile
		writestr(STDOUT_FILENO, "What is the secret in this pipe ?\n");
		writestr(STDOUT_FILENO, "Son : \"");

		while (read(pipefd[0], &buf, 1) > 0)
		{
			write(STDOUT_FILENO, &buf, 1);
		}
		writestr(STDOUT_FILENO, "\"\n");

		close(pipefd[0]);
		exit(EXIT_SUCCESS);
	}
	else
		close(pipefd[0]);
	writestr(pipefd[1], "I am your father");
	close(pipefd[1]);
	wait(NULL);
	writestr(STDOUT_FILENO, "Father : Hey son\n");
	exit(EXIT_SUCCESS);
}