#include "../inc/minishell.h"

//For testing purpose

static char *get_token_type_name(t_token_type token_type)
{
	switch (token_type)
	{
		case WORD_SINGLE: return "WORD_S";
		case WORD_DOUBLE: return "WORD_D";
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

int main()
{
	char *input_str;
	t_input input;

	while (1)
	{
		input_str = readline(PROMPT);
		if (!input_str)
			break ;
		add_history(input_str);
		if (!input_validation(input_str))
		{
			// expand(input_str);
			// concatinate_adjacecnt_quotes(input_str);
			init_lexer(&input, input_str);
			extract_token(&input);
			print_tokens(input.tokens);
			if (tokens_validation(input.tokens) == SUCCESS)
			{
				retokenize_words(input.tokens);
				print_tokens(input.tokens);
				free(input_str);
			}
		}
	}
	return (0);
}
