/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:27:44 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/30 14:45:23 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check_files(int argc, char **argv)
{
	int	input_fd;
	int	output_fd;

	if (argc < 5)
	{
		ft_printf("Not enough arguments\n");
		return (EXIT_FAILURE);
	}
	input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1)
	{
		perror("Error, opening of entry file impossible\n");
		exit(EXIT_FAILURE);
	}
	output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
	{
		perror("Error, something was wrong with the output file\n");
		close(input_fd);
		exit(EXIT_FAILURE);
	}
	close(input_fd);
	close(output_fd);
	return (EXIT_SUCCESS);
}

char	**get_cmd(char *argv)
{
	char	**cmd;

	cmd = ft_split(argv, ' ');
	if (cmd == NULL)
	{
		return (NULL);
	}
	return (cmd);
}

void	free_path(char **path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
}
