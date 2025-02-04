/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:23:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/04 16:01:30 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd_list;
	t_data	data;

	cmd_list = NULL;
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
		return (1);
	}
	// cmd_list = NULL;
	init_data(&data, argv, argc);
	init_cmd(cmd_list, &data);
	printf("%s", cmd_list->data->file1);
	return (0);
	cmd_list = get_commands(argc, argv, envp);
	execute_commands(cmd_list, envp);
	free_cmd_list(cmd_list);
	return (0);
}
