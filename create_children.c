/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_children.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:09:11 by lisux             #+#    #+#             */
/*   Updated: 2025/02/03 10:59:58 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int count_commands(t_cmd *cmds)
{
    int count = 0;
    while (cmds)
    {
        count++;
        cmds = cmds->next;
    }
    return count;
}

void execute_commands(t_cmd *cmds, int num_cmds, char **envp)
{
    int pipes[num_cmds - 1][2];
    pid_t pids[num_cmds];
    t_cmd *current = cmds;
    int i = 0;

    // Création des pipes (nb de cmd - 1)
    while (i < num_cmds - 1)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe error");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    i = 0;
    while (current)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) // Processus fils
        {
            // Première commande : rediriger l'entrée depuis file1
            if (i == 0)
            {
                int input_fd = open("file1", O_RDONLY);
                if (input_fd == -1)
                {
                    perror("open error");
                    exit(EXIT_FAILURE);
                }
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            // Dernière commande : rediriger la sortie vers file2
            else if (i == num_cmds - 1)
            {
                int output_fd = open("file2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd == -1)
                {
                    perror("open error");
                    exit(EXIT_FAILURE);
                }
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                dup2(output_fd, STDOUT_FILENO);
            }
            // Commande du milieu : relier entrée et sortie aux pipes
            else
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Fermer tous les pipes dans chaque processus fils
            for (int j = 0; j < num_cmds - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Exécuter la commande
            execve(current->path, current->args, envp);
            perror("execve error");
            exit(EXIT_FAILURE);
        }

        current = current->next; // Passer au maillon suivant
        i++;
    }

    // Fermeture des pipes dans le processus parent
    for (int j = 0; j < num_cmds - 1; j++)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    // Attendre tous les processus enfants
    for (int k = 0; k < num_cmds; k++)
        waitpid(pids[k], NULL, 0);
}

