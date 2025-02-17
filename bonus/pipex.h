/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:16:40 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/14 14:04:20 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "LIBFT/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_cmd
{
	char			**args;
	char			*path;
	int				num_cmds;
	char			*file1;
	char			*file2;
	struct s_cmd	*head;
	struct s_cmd	*next;
}					t_cmd;

char				*get_env(char **envp);
char				*find_path(char *env, char *cmd);
void				free_path(char **path);
//-------------------------------- cmd list
t_cmd				*get_commands(int argc, char **argv, char **envp);
void				free_cmd_list(t_cmd *cmd_list);
t_cmd				*cmd_new(char *cmd_str, char *envp[]);
void				cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd);
void				add_count_files(t_cmd *cmds, char **argv, int argc);
//------------------------------------------ pipes and exec cmd
void				pipex(t_cmd *cmds, char **envp);
//-----------utils------------------------
void				create_pipes(t_cmd *cmds, int (*pipes)[2]);
void				close_pipes(int (*pipes)[2], int num_cmds);
void				create_kids(pid_t *pids, t_cmd *cmds, int (*pipes)[2]);
void				wait_for_kids(int num_cmds);
void				free_all(int (*pipes)[2], t_cmd *cmds);
void				file2_rights(int *fd2, t_cmd *cmds);
void				error_message(t_cmd *current, int fd2);
void				error_exit(const char *msg, int code);
#endif