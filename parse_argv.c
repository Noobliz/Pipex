/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:27:44 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/06 12:47:08 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_path(char **path)
{
	int	i;

	i = 0;
	if (!path)
		return ;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
}
void	error_exit(const char *msg, int code)
{
	perror(msg);
	exit(code);
}
// int	check_files(int argc, char **argv)
// {
// 	int	input_fd;
// 	int	output_fd;

// 	if (argc < 5)
// 	{
// 		ft_printf("Not enough arguments\n");
// 		return (EXIT_FAILURE);
// 	}
// 	input_fd = open(argv[1], O_RDONLY);
// 	if (input_fd == -1)
// 	{
// 		perror("Error, opening of entry file impossible\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (output_fd == -1)
// 	{
// 		perror("Error, something was wrong with the output file\n");
// 		close(input_fd);
// 		exit(EXIT_FAILURE);
// 	}
// 	close(input_fd);
// 	close(output_fd);
// 	return (EXIT_SUCCESS);
// }

// NOUVELLE VERSION
void	check_files(char *file1, char *file2)
{
	int	fd;

	fd = open(file1, O_RDONLY);
	if (fd == -1)
		ft_printf("pipex: %s\n", file1);
	else
		close(fd);
	fd = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_exit(file2, 1);
	close(fd);
}

// void	init_data(t_data *data, char **argv, int argc)
// {
// 	data->file1 = ft_strdup(argv[1]);
// 	data->file2 = ft_strdup(argv[argc - 1]);
// }
// void	init_cmd(t_cmd *cmd, t_data *data)
// {
// 	cmd->args = NULL;
// 	cmd->path = NULL;
// 	cmd->num_cmds = 0;
// 	cmd->data = data;
// 	cmd->next = NULL;
// }

// void	free_path(char **path)
// {
// 	int	i;

// 	i = 0;
// 	while (path[i])
// 	{
// 		free(path[i]);
// 		i++;
// 	}
// 	free(path);
// }
