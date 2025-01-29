

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void	writestr(int fd, const char *str)
{
	write(fd, str, strlen(str));
}
int check_files(int argc, char **argv)
{
	 if (argc != 5) 
	{
        fprintf(stderr, "Usage : %s file1 cmd1 cmd2 file2\n", argv[0]);
        return EXIT_FAILURE;
    }

    int input_fd;
	input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) 
	{
        perror("Error, opening of entry file impossible_n");
        return EXIT_FAILURE;
    }

    int output_fd;
	output_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Error, something was wrong with the output file");
        close(input_fd);
        return EXIT_FAILURE;
    }

    close(input_fd);
    close(output_fd);

    return EXIT_SUCCESS;
}
int main(int argc, char **argv) 
{
   //if(!check_files(argc, argv))
	//return(0);

	int	pipefd[2];
	pid_t pid1;
	pid_t pid2;
	char *cmd1[] = {"/bin/cat", NULL};

	if(pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	int input_fd;
	input_fd = open(argv[1], O_RDONLY);

	// create fork for cmd1

	if((pid1 = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}

	// fils de cmd1
	if (pid1 == 0)
	{
		close(pipefd[0]); // fermer la lecture dans fils de cm1

		dup2(input_fd, STDIN_FILENO); // redirige stdin vers file1
		close(input_fd); // car il a été dupliqué

		//rediriger stdout vers le pipe
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		// executer cmd1
		execve(cmd1[0], cmd1, NULL);
		perror("execve cmd1");
		exit(1);

	}
	if ((pid2 = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}

	// fils de cmd2

	if (pid2 == 0)
	{
		close(pipefd[1]); //ferme l'écriture

		dup2(pipefd[0], STDIN_FILENO); //fait pointer entrée stand vers pipe de lecture
		close(pipefd[0]);

		char *cmd2[] = {"/usr/bin/wc", "-l", NULL};

		execve(cmd2[0], cmd2, NULL);
		perror("exercve cmd2");
		exit(1);
	}

	   // Processus père
    // Fermer les descripteurs du pipe dans le père
    close(pipefd[0]);
    close(pipefd[1]);

	    // Attendre la fin des deux processus fils
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0); 
}
