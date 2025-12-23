#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

extern jmp_buf errorHndlr;

void exec_error(const char *msg) {
    perror(msg);
    longjmp(errorHndlr, 1);
}

int execute_command(tree cmd, int input_fd, int output_fd, int is_background) {
    if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL) {
        return 0;
    }

    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argv[1] == NULL) {
            const char *home = getenv("HOME");
            if (home == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
            if (chdir(home) != 0) {
                perror("cd");
                return 1;
            }
        } else {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd");
                return 1;
            }
        }
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        exec_error("fork failed");
        return 1;
    }

    if (pid == 0) {
        if (cmd->infile != NULL) {
            int fd = open(cmd->infile, O_RDONLY);
            if (fd < 0) {
                perror("open infile");
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) < 0) {
                perror("dup2 infile");
                exit(1);
            }
            close(fd);
        } else if (input_fd != STDIN_FILENO) {
            if (dup2(input_fd, STDIN_FILENO) < 0) {
                perror("dup2 pipe input");
                exit(1);
            }
            close(input_fd);
        }

        if (cmd->outfile != NULL) {
            int flags = O_WRONLY | O_CREAT;
            if (cmd->append) {
                flags |= O_APPEND;
            } else {
                flags |= O_TRUNC;
            }
            int fd = open(cmd->outfile, flags, 0644);
            if (fd < 0) {
                perror("open outfile");
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2 outfile");
                exit(1);
            }
            close(fd);
        } else if (output_fd != STDOUT_FILENO) {
            if (dup2(output_fd, STDOUT_FILENO) < 0) {
                perror("dup2 pipe output");
                exit(1);
            }
            close(output_fd);
        }

        if (is_background) {
            int null_fd = open("/dev/null", O_WRONLY);
            if (null_fd >= 0) {
                dup2(null_fd, STDERR_FILENO);
                close(null_fd);
            }
        }

        execvp(cmd->argv[0], cmd->argv);
        
        perror(cmd->argv[0]);
        exit(1);
    } else {
        if (!is_background) {
            int status;
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                fprintf(stderr, "Command terminated by signal %d\n", WTERMSIG(status));
                return 128 + WTERMSIG(status);
            }
            return 1;
        } else {
            printf("[%d] %s\n", pid, cmd->argv[0]);
            return 0;
        }
    }
}

int execute_pipe_chain(tree cmd, int input_fd, int is_background) {
    if (cmd == NULL)
        return 0;

    int exit_status = 0;
    tree current = cmd;
    int prev_pipe_read = input_fd;
    int pipe_fd[2];
    
    while (current != NULL) {
        if (current->pipe != NULL) {
            if (pipe(pipe_fd) < 0) {
                exec_error("pipe failed");
                return 1;
            }
        }
        
        int output_fd = (current->pipe != NULL) ? pipe_fd[1] : STDOUT_FILENO;
        
        int bg = (current->pipe == NULL) ? is_background : 0;
        
        exit_status = execute_command(current, prev_pipe_read, output_fd, bg);
        
        if (prev_pipe_read != STDIN_FILENO) {
            close(prev_pipe_read);
        }
        if (output_fd != STDOUT_FILENO) {
            close(output_fd);
        }
        
        if (current->pipe != NULL) {
            prev_pipe_read = pipe_fd[0];
        }
        
        current = current->pipe;
    }
    
    return exit_status;
}

int execute(tree t) {
    if (t == NULL)
        return 0;
    if (strcmp(*t->argv, "exit") == 0)
        return 1;
    
    tree current = t;
    int last_exit_status = 0;
    
    while (current != NULL) {
        int should_execute = 1;
        
        if (current != t) {
            tree prev_cmd = t;
            while (prev_cmd->next != current)
                prev_cmd = prev_cmd->next;
            
            if (prev_cmd->type == AND)
                should_execute = (last_exit_status == 0);
            else if (prev_cmd->type == OR)
                should_execute = (last_exit_status != 0);
        }
        
        if (should_execute)
            last_exit_status = execute_pipe_chain(current, STDIN_FILENO, current->backgrnd);
        
        current = current->next;
    }

    return 0;
}