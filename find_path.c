/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:18:05 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/30 15:10:24 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env(char **envp)
{
	char	*env;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (envp[i])
	{
		env = ft_strnstr(envp[i], "PATH=", 5);
		if (env)
		{
			while (j < 5)
			{
				env++;
				j++;
			}
			return (env);
		}
		i++;
	}
	return (NULL);
}
char	*add_cmd(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*arr;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	arr = malloc(s1_len + s2_len + 2);
	if (!arr)
		return (NULL);
	ft_strlcpy(arr, s1, s1_len + 1);
	arr[s1_len] = '/';
	arr[s1_len + 1] = '\0';
	ft_strlcat(arr, s2, s2_len + s1_len + 2);
	return (arr);
}
char	*find_path(char *env, char *cmd)
{
	char	**path;
	char	*path_found;
	int		i;

	i = 0;
	path = ft_split(env, ':');
	while (path && path[i])
	{
		path_found = add_cmd(path[i], cmd);
		if ((access(path_found, F_OK) == 0) && (access(path_found, X_OK) == 0))
		{
			free_path(path);
			printf("for '%s', the path is :%s\n", cmd, path_found);
			return (path_found);
		}
		i++;
		if (path[i])
			free(path_found);
	}
	if (path && *path)
		free_path(path);
	free(path_found);
	return (NULL);
}
