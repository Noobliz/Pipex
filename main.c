/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:23:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/30 14:32:38 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

int	main(int argc, char **argv, char **envp)
{
	char	*env;
	char	**cmd;
	int		i;

	// char	*cmd_found;
	i = 0;
	if (!argc || !argv)
		return (0);
	env = get_env(envp);
	cmd = get_cmd(argv[2]);
	cmd[0] = find_path(env, cmd[0]);
	ft_printf("%s\n%s\n", cmd[0], cmd[1]);
	// if (!cmd_found)
	// 	ft_printf("Command not found\n");
	// free(cmd_found);
	return (0);
}
