/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:44:59 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/03 16:27:08 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		free_path(cmd_list->args);
		free(cmd_list->path);
		free(cmd_list);
		cmd_list = tmp;
	}
}
t_cmd	*cmd_new(char *cmd_str, char *envp[])
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = ft_split(cmd_str, ' ');
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
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
void	count_commands(t_cmd *cmds)
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
		current->num_cmds = count;
		current = current->next;
	}
}
