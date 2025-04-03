#include "../inc/minishell.h"

static char	*add_command(t_token *tokens)
{
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == COMMAND)
			return (tokens->value);
		tokens = tokens->next;
	}
	return (NULL);
}

static char	**find_cmd_and_compose(t_token *tokens)
{
	char	*arg;
	char	**cmd;
	int		i;

	i = 0;
	cmd = (char **)malloc((count_amount_cmd(tokens) + 1) * sizeof(char *));
	// if (!cmd)
	// 	exit_error(MALLOC); to-do
	cmd[i] = add_command(tokens);
	tokens = tokens->next;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == ARG)
			cmd[++i] = tokens->value;
		tokens = tokens->next;
	}
	cmd[++i] = NULL;
	return (cmd);
}

static void	build_branch_add_redirects(t_arena **arena, t_ast **ast, t_token *tokens, t_branch branch)
{
	t_token	*tmp;

	tmp = tokens;
	if (branch == LAST_BRANCH)
	{
		while (tmp && tmp->type != PIPE)
		{
			if (is_any_redirect(tmp->type))
			{
				add_node(ast, init_node(arena, NULL, tmp->next->value, tmp->type), FIRST);
				tmp = tmp->next;
				break ;
			}
			tmp = tmp->next;
		}
	}
	while (tmp && tmp->type != PIPE)
	{
		if (is_any_redirect(tmp->type))
			add_node(ast, init_node(arena, NULL, tmp->next->value, tmp->type), NON_FIRST);
		tmp = tmp->next;
	}
}

static void	build_branch_add_command(t_arena **arena, t_ast **ast, t_token *tokens, t_branch branch)
{
	t_token	*tmp;
	char	**cmd;

	tmp = tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == COMMAND)
		{
			cmd = find_cmd_and_compose(tmp);
			if (branch == LAST_BRANCH && last_is_pipe(ast))
				add_node(ast, init_node(arena, cmd, NULL, tmp->type), FIRST);
			else
				add_node(ast, init_node(arena, cmd, NULL, tmp->type), NON_FIRST);
		}
		tmp = tmp->next;
	}
}

void	build_branch(t_arena **arena, t_ast **ast, t_token *tokens, t_branch branch)
{
	build_branch_add_redirects(arena, ast, tokens, branch);
	build_branch_add_command(arena, ast, tokens, branch);
}
