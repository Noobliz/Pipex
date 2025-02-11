/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:44:59 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/10 17:10:44 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;
	int		a;

	a = 0;
	while (cmd_list)
	{
		tmp = cmd_list->next;
		if (cmd_list->args)
			free_path(cmd_list->args);
		if (cmd_list->path)
			free(cmd_list->path);
		free(cmd_list);
		cmd_list = tmp;
	}
}
// NOUVELLE VERSION
t_cmd	*cmd_new(char *cmd_str, char *envp[])
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->path = NULL;
	cmd->args = ft_split(cmd_str, ' ');
	if (!cmd->args || !cmd->args[0])
	{
		free(cmd);
		return (NULL);
	}
	// SI YA UN CHEMIN ABSOLU
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
	{
		if (access(cmd->args[0], X_OK) == 0)
			cmd->path = ft_strdup(cmd->args[0]);
	}
	else
		cmd->path = find_path(get_env(envp), cmd->args[0]);
	cmd->next = NULL;
	return (cmd);
}

void	cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (!cmd_list || !new_cmd)
		return ;
	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	tmp = *cmd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
}
void	count_commands(t_cmd *cmds, char **argv, int argc)
{
	int		count;
	t_cmd	*current;

	current = cmds;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	current = cmds;
	while (current)
	{
		current->file1 = argv[1];
		current->file2 = argv[argc - 1];
		current->num_cmds = count;
		current = current->next;
	}
}
