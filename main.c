/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:23:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/12 14:00:45 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd_list;

	cmd_list = NULL;
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
		return (1);
	}
	cmd_list = get_commands(argc, argv, envp);
	//_printf("num cmd : %d\n", cmd_list->num_cmds);
	pipex(cmd_list, envp);
	// free_cmd_list(cmd_list);
	return (0);
}
