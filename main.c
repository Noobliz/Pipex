/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:23:47 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/29 16:28:28 by lguiet           ###   ########.fr       */
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
	char	*cmd_found;
	int		i;

	i = 0;
	if (!argc || !argv)
		return (0);
	env = get_env(envp);
	cmd_found = find_path(env, "cat");
	free(cmd_found);
	return (0);
}
