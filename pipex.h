/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:16:40 by lguiet            #+#    #+#             */
/*   Updated: 2025/02/04 15:57:01 by lguiet           ###   ########.fr       */
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

typedef struct s_data
{
	char			*file1;
	char			*file2;
}					t_data;

typedef struct s_cmd
{
	char			**args;
	char			*path;
	int				num_cmds;
	struct s_data	*data;
	struct s_cmd	*next;
}					t_cmd;

int					check_files(int argc, char **argv);
void				init_data(t_data *data, char **argv, int argc);
void				init_cmd(t_cmd *cmd, t_data *data);
char				*get_env(char **envp);
char				*find_path(char *env, char *cmd);
void				free_path(char **path);

//-------------------------------- cmd list

t_cmd				*get_commands(int argc, char **argv, char **envp);
void				free_cmd_list(t_cmd *cmd_list);
void				free_cmd_list(t_cmd *cmd_list);
t_cmd				*cmd_new(char *cmd_str, char *envp[]);
void				cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd);
void				count_commands(t_cmd *cmds);

//------------------------------------------ pipes and exec cmd
void				execute_commands(t_cmd *cmds, char **envp);
//-----------utils------------------------
void				create_pipes(int num_cmds, int (*pipes)[2]);
void				close_pipes(int (*pipes)[2], int num_cmds);
void				create_kids(int i, pid_t *pids);
void				wait_for_kids(int num_cmds, pid_t *pids);
#endif