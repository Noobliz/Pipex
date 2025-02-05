/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:28:30 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/05 14:28:56 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env(char **envp)
{
	int	i;

	i = 0;
	// SI ON UNSET LE PATH SAMEEER
	   if (!envp || !envp[0])
    {
        fprintf(stderr, "pipex: error: no environment found\n");
        exit(1);
    }
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*add_cmd(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*arr;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	arr = malloc(s1_len + s2_len + 2);
	if (!arr)
		return (NULL);
	ft_strlcpy(arr, s1, s1_len + 1);
	arr[s1_len] = '/';
	ft_strlcpy(arr + s1_len + 1, s2, s2_len + 1);
	return (arr);
}

char	*find_path(char *env, char *cmd)
{
	char	**path;
	char	*full_path;
	int		i;

	i = 0;
	if (!env || !cmd)
		return (NULL);
	path = ft_split(env, ':');
	if (!path)
		return (NULL);
	while (path[i])
	{
		full_path = add_cmd(path[i], cmd);
		if (!full_path)
			continue ;
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
		{
			free_path(path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_path(path);
	return (NULL);
}

t_cmd	*get_commands(int argc, char **argv, char **envp)
{
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;
	int		i;

	cmd_list = NULL;
	i = 2;
	while (i < argc - 1) // Ne pas inclure file1 et file2
	{
		new_cmd = cmd_new(argv[i], envp);
		if (!new_cmd)
		{
			free_cmd_list(cmd_list);
			return (NULL);
		}
		cmd_add_back(&cmd_list, new_cmd);
		i++;
	}
	count_commands(cmd_list, argv, argc);
	return (cmd_list);
}
