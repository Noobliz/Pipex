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

typedef struct s_cmd
{
	char **args;          // Arguments de la commande
	char *path;           // Chemin complet trouvé avec find_path()
	struct s_cmd *next;   // Commande suivante
} t_cmd;


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

t_cmd	*cmd_new(char *cmd_str, char *envp[])
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);

    // Parser la commande (split sur les espaces)
    cmd->args = ft_split(cmd_str, ' ');
    if (!cmd->args)
    {
        free(cmd);
        return (NULL);
    }

    // Trouver le chemin du binaire
    cmd->path = find_path(get_env(envp), cmd->args[0]);
    cmd->next = NULL;
    
    return (cmd);
}

void cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd)
{
    t_cmd *tmp;

    if (!cmd_list || !new_cmd)
        return;

    if (!*cmd_list) // Si la liste est vide
    {
        *cmd_list = new_cmd;
        return;
    }

    tmp = *cmd_list;
    while (tmp->next) // Aller au dernier élément
        tmp = tmp->next;
    
    tmp->next = new_cmd;
}
t_cmd *parse_commands(int argc, char *argv[], char *envp[])
{
    t_cmd *cmd_list = NULL;
    t_cmd *new_cmd;
    int i = 2; // Les commandes commencent à argv[2]

    while (i < argc - 1) // Ne pas inclure file1 et file2
    {
        new_cmd = cmd_new(argv[i], envp);
        if (!new_cmd)
        {
            // En cas d'erreur, libérer toute la liste
            free_cmd_list(cmd_list);
            return (NULL);
        }
        cmd_add_back(&cmd_list, new_cmd);
        i++;
    }
    return (cmd_list);
}

void free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *tmp;

    while (cmd_list)
    {
        tmp = cmd_list->next;
        free_path(cmd_list->args); // Libérer le tableau d'arguments
        free(cmd_list->path);      // Libérer le chemin de la commande
        free(cmd_list);
        cmd_list = tmp;
    }
}

int main(int argc, char *argv[], char *envp[])
{
    if (argc < 5)
    {
        fprintf(stderr, "Usage: ./pipex file1 cmd1 cmd2 file2\n");
        return (1);
    }

    t_cmd *cmd_list = parse_commands(argc, argv, envp);
    if (!cmd_list)
    {
        fprintf(stderr, "Erreur lors du parsing des commandes.\n");
        return (1);
    }

    // Affichage pour vérifier
t_cmd *tmp = cmd_list;
while (tmp)
{
    printf("Commande : %s\n", tmp->args[0]);
    int i = 1;
    while (tmp->args[i])
    {
        printf("Option %d : %s\n", i, tmp->args[i]);
        i++;
    }
    printf("Chemin trouvé : %s\n\n", tmp->path);
    tmp = tmp->next;
}


    free_cmd_list(cmd_list);
    return (0);
}



