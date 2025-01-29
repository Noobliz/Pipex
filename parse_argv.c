/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:27:44 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/29 15:31:27 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	writestr(int fd, const char *str)
{
	write(fd, str, strlen(str));
}
int	check_files(int argc, char **argv)
{
	int	input_fd;
	int	output_fd;

	if (argc != 5)
	{
		fprintf(stderr, "Usage : %s file1 cmd1 cmd2 file2\n", argv[0]);
		return (EXIT_FAILURE);
	}
	input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1)
	{
		perror("Error, opening of entry file impossible_n");
		return (EXIT_FAILURE);
	}
	output_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
	{
		perror("Error, something was wrong with the output file");
		close(input_fd);
		return (EXIT_FAILURE);
	}
	close(input_fd);
	close(output_fd);
	return (EXIT_SUCCESS);
}
