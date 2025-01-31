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
	int i = 0;

	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5); // Retourner juste après "PATH="
		i++;
	}
	return (NULL);
}

char	*add_cmd(char const *s1, char const *s2)
{
	if (!s1 || !s2)
		return (NULL);

	size_t s1_len = ft_strlen(s1);
	size_t s2_len = ft_strlen(s2);
	char *arr = malloc(s1_len + s2_len + 2);
	if (!arr)
		return (NULL);
	
	ft_strlcpy(arr, s1, s1_len + 1);
	arr[s1_len] = '/';
	ft_strlcpy(arr + s1_len + 1, s2, s2_len + 1);
	
	return (arr);
}

char	*find_path(char *env, char *cmd)
{
	if (!env || !cmd)
		return (NULL);

	char **path = ft_split(env, ':');
	if (!path)
		return (NULL);

	char *path_found = NULL;
	for (int i = 0; path[i]; i++)
	{
		char *full_path = add_cmd(path[i], cmd);
		if (!full_path)
			continue;

		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
		{
			free_path(path);
			return (full_path);
		}
		free(full_path);
	}

	free_path(path);
	return (NULL);
}

