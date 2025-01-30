/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:16:40 by lguiet            #+#    #+#             */
/*   Updated: 2025/01/30 16:16:32 by lguiet           ###   ########.fr       */
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
	char			*cmd;
	char			**cmd_opt;
	char			*env;
	char			**argv;
	struct s_data	*next;
}					t_data;

int					check_files(int argc, char **argv);
char				*get_env(char **envp);
char				*find_path(char *env, char *cmd);
void				free_path(char **path);
char				**get_cmd(char *argv);
#endif