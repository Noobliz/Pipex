#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void execute_command(char *cmd, char **envp)
{
    char *args[] = {cmd, NULL};  // Ici, il faudrait parser `cmd` en arguments
    execve(cmd, args, envp);
    perror("execve failed");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
    if (argc < 5) // ./pipex file1 cmd1 cmd2 ... cmdN file2
    {
        fprintf(stderr, "Usage: %s file1 cmd1 cmd2 ... cmdN file2\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int num_cmds = argc - 3; // On enlève file1 et file2
    int pipes[num_cmds - 1][2]; // On a besoin de num_cmds - 1 pipes
    int pids[num_cmds];
    int file_in, file_out;

    // Ouvrir file1 en lecture
    file_in = open(argv[1], O_RDONLY);
    if (file_in < 0) {
        perror("Error opening file1");
        return (EXIT_FAILURE);
    }

    // Ouvrir file2 en écriture
    file_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_out < 0) {
        perror("Error opening file2");
        return (EXIT_FAILURE);
    }

    // Créer tous les pipes nécessaires
    for (int i = 0; i < num_cmds - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            return (EXIT_FAILURE);
        }
    }

    // Boucle pour créer chaque processus
    for (int i = 0; i < num_cmds; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            return (EXIT_FAILURE);
        }

        if (pids[i] == 0) // Processus enfant
        {
            // Première commande : entrée = file1
            if (i == 0)
                dup2(file_in, STDIN_FILENO);
            else
                dup2(pipes[i - 1][0], STDIN_FILENO); // Lire depuis le pipe précédent

            // Dernière commande : sortie = file2
            if (i == num_cmds - 1)
                dup2(file_out, STDOUT_FILENO);
            else
                dup2(pipes[i][1], STDOUT_FILENO); // Écrire dans le pipe suivant

            // Fermer tous les pipes
            for (int j = 0; j < num_cmds - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            close(file_in);
            close(file_out);
            execute_command(argv[i + 2], envp);
        }
    }

    // Fermer tous les pipes dans le parent
    for (int i = 0; i < num_cmds - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    close(file_in);
    close(file_out);

    // Attendre tous les enfants
    for (int i = 0; i < num_cmds; i++)
        waitpid(pids[i], NULL, 0);

    return (EXIT_SUCCESS);
}
