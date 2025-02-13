/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:27:44 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/13 14:51:31 by lguiet           ###   ########.fr       */
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

void	error_message(t_cmd *current)
{
	if (!current->args)
		ft_printf("pipex: Command not found\n");
	if (!current->path && current->args)
		perror("pipex he");
	else if (current->args && current->path)
		perror("pipex");
}

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
