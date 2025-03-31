#include "../inc/minishell.h"

//For testing purpose

char *get_token_type_name(t_token_type token_type)
{
	switch (token_type)
	{
		case WORD: return "WORD";
		case COMMAND: return "COMMAND";
		case ARG: return "ARG";
		case FILE_TOKEN: return "FILE";
		case HERE_TOKEN: return "HERE_WORD";
		case PIPE: return "PIPE";
		case REDIRECT_IN: return "REDIRECT_IN";
		case REDIRECT_OUT: return "REDIRECT_OUT";
		case REDIRECT_APPEND: return "REDIRECT_APPEND";
		case HERE_STRING: return "HERE_STRING";
		case ENV_VAR: return "ENV_VAR";
		default: return "UNKNOWN";
	}
}

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->value)
		{
			printf("Type: %15s, Length of token: %d, Value: %.*s\n", get_token_type_name(tokens->type), tokens->len, tokens->len, tokens->value);
		}
		tokens = tokens->next;
	}
	printf("\n");
}
//Testing ends

int init_minishell(t_minishell *mshell, char **envp)
{
	if (copy_env(mshell->envp, envp))
		return (FAIL);
	return (SUCCESS);
}

int main(int ac, char **av, char **envp)
{
	char		*input_str;
	t_input		input;
	t_ast		*ast;
	t_minishell	mshell;

	if (init_minishell(&mshell, envp))
		return (FAIL);
		// exit_error(init_issue)
	while (1)
	{
		input_str = readline(PROMPT);
		if (!input_str)
			break ;
		add_history(input_str);
		if (!input_validation(input_str))
		{
			// expand(input_str);//double quote or single quote expasion
			init_lexer(&input, input_str);
			extract_token(&input);
			if (tokens_validation(input.tokens) == SUCCESS)
			{
				retokenize_words(input.tokens);
				handle_heredoc(mshell.envp->envp, input.tokens);
				print_tokens(input.tokens);
				// ast = build_ast_binary_tree(input.tokens);
				//ast tree
				free(input_str); // dont free this before the whole program ends!
			}
		}
	}
	return (0);
}

// ls -la < file1 > file1.1| cat -e >file2 <<file3 | grep filename >>file4 | du -s > file5
