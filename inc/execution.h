#ifndef EXECUTION_H
# define EXECUTION_H

typedef struct s_env t_env;
typedef struct s_ast t_ast;
typedef struct s_minishell t_minishell;


# define FD_IN  0
# define FD_OUT 1

typedef struct s_execution
{
	char 	**cmd_args;   // Command and its arguments
	int		redir_fd[2];        // temp_fd[FD_IN] = input, temp_fd[FD_OUT] = output
	int		prev_fd;
	int		pipefd[2];
	int		has_next_pipe;
	int		has_pipe;

}				t_execution;

char	*get_command_path(t_minishell *mshell, t_ast *ast);
void	execute_ast(t_minishell *mshell, t_ast *ast);
int		wait_for_children(t_minishell *mshell);
void		setup_child_fds(int prev_fd, int *pipefd, int has_next_pipe, t_execution *exec);
int			execute_builtin_parent(t_minishell *mshell, t_execution *exec);
void		execute_builtin_child(t_minishell *mshell, t_execution *exec);
int			redirect_and_restore_builtin(t_execution *exec, t_minishell *mshell, char **cmd_args);



void execute_ast_v1(t_minishell *mshell, t_ast *ast);

//NEW! REFACTOR!


//redirection
int	handle_redirection(t_ast *ast);
int	safe_dup2(int fd, int target_fd);
int	open_redir_file(const char *file, int flags, int *fd);


//execute builtins
t_ast	*get_cmd_node(t_ast *ast);
int		is_builtin(t_ast *ast);
int execute_builtin(t_minishell *mshell, t_ast *ast);




#endif

