#include "../../inc/minishell.h"

static int safe_open_redir_in(t_execution *exec, char *filepath)
{
	if (exec->redir_fd[FD_IN] != STDIN_FILENO)
		close(exec->redir_fd[0]);

	exec->redir_fd[FD_IN] = open(filepath, O_RDONLY, 0);
	if (exec->redir_fd[FD_IN] == -1)
	{
		ft_dprintf(2, "Giraffeshell: ");
		perror(filepath);
		return(FAIL);
	}
	return SUCCESS;
}

static int safe_open_redir_out(t_execution *exec, char *filepath)
{
	if (exec->redir_fd[FD_OUT] != STDOUT_FILENO )
		close(exec->redir_fd[FD_OUT]);

	exec->redir_fd[FD_OUT] = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (exec->redir_fd[FD_OUT] == -1)
	{
		ft_dprintf(2, "Giraffeshell: ");
		perror(filepath);
		return(FAIL);
	}
	return SUCCESS;
}

static int safe_open_redir_append(t_execution *exec, char *filepath)
{
	if (exec->redir_fd[FD_OUT] != STDOUT_FILENO)
		close(exec->redir_fd[FD_OUT]);

	exec->redir_fd[FD_OUT] = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (exec->redir_fd[FD_OUT] == -1)
	{
		ft_dprintf(2, "Giraffeshell: ");
		perror(filepath);
		return FAIL;
	}
	return SUCCESS;
}

int handle_redirections(t_ast *node, t_execution *exec)
{
	while (node)
	{
		if (node->type == REDIRECT_IN || node->type == HERE_DOCUMENT)
		{
			if (safe_open_redir_in(exec, node->file) == FAIL)
				return (FAIL);
		}
		else if (node->type == REDIRECT_OUT)
		{
			if (safe_open_redir_out(exec, node->file) == FAIL)
				return (FAIL);
		}
		else if (node->type == REDIRECT_APPEND)
		{
			if (safe_open_redir_append(exec, node->file) == FAIL)
				return (FAIL);
		}
		node = node->next_left;
	}
	return (SUCCESS);
}

static t_ast *get_cmd_node(t_ast *ast_branch)
{
	t_ast *current = ast_branch; // Start from the main node (PIPE or first command)
	// Traverse the left branch (command/redirections)
	while (current)
	{
		if (current->type == COMMAND)
			return (current); // Found the command node
		current = current->next_left;
	}
	//ft_dprintf(2, "Debug: get_cmd_node did NOT find a COMMAND node.\n"); // Debug print
	return (NULL); // No command node found in this branch
}

int travel_left(t_minishell *mshell, t_execution *exec, t_ast *ast)
{
	t_ast *cmd_node;

	if (handle_redirections(ast, exec) == FAIL)
	{
		mshell->exitcode = 1;
		delete_minishell(mshell);
		exit(mshell->exitcode);
	}

	cmd_node = get_cmd_node(ast);
	if (cmd_node)
		exec->cmd_args = cmd_node->cmd;
	return (SUCCESS);
}


