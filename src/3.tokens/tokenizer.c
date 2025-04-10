#include "minishell.h"

void	init_lexer(t_input *new_input, char *input_str)
{
	new_input->full_str = input_str;
	new_input->index = 0;
	new_input->len = ft_strlen(input_str);
	new_input->tokens = NULL;
}

static t_token *init_token(t_minishell *mshell, t_input *input, int len, t_token_type type)
{
	t_token *new_token;

	// new_token = (t_token *)malloc(sizeof(t_token));
	new_token = arena_alloc(mshell->arena, sizeof(t_token), alignof(t_token));
	// if (!new_token)
		// exit_error(MALLOC);
	new_token->value = &input->full_str[input->index];
	new_token->len = len;
	new_token->type = type;
	new_token->next = NULL;
	input->index += len;
	return (new_token);
}

static t_token *init_token_word(t_minishell *mshell, char *word, t_token_type type)
{
	t_token *new_token;

	// new_token = (t_token *)malloc(sizeof(t_token));
	new_token = arena_alloc(mshell->arena, sizeof(t_token), alignof(t_token));
	// if (!new_token)
		// exit_error(MALLOC);
	new_token->value = &word[0];
	new_token->len = ft_strlen(word);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

static void add_token(t_token **head, t_token *new)
{
	t_token *last;
	if (new == NULL)
		return;
	if (*head == NULL)
		*head = new;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

void	word(t_minishell *mshell, t_input *input)
{
	t_quotes_helper	quotes;
	char			*input_str;
	char			*new_str;

	input_str = input->full_str;
	init_quotes(&quotes);
	new_str = ft_strdup("");
	while (is_valid_char(&quotes, input))
	{
		update_quote_state(input->full_str[input->index], &quotes);
		append_char(input_str, &new_str, input->index);
		input->index++;
	}
	add_token(&input->tokens, init_token_word(mshell, new_str, WORD));
}

static void	create_tokens(t_minishell *mshell, t_input *input)
{
	while (input->index < input->len)
	{
		while (input->full_str[input->index] == ' ')
			input->index++;
		if (input->full_str[input->index] == '\0')
			break ;
		if (ft_strncmp(&input->full_str[input->index], "<<", 2) == 0)
			add_token(&input->tokens, init_token(mshell, input, 2, HERE_DOCUMENT));
		else if (ft_strncmp(&input->full_str[input->index], ">>", 2) == 0)
			add_token(&input->tokens, init_token(mshell, input, 2, REDIRECT_APPEND));
		else if (input->full_str[input->index] == '|')
			add_token(&input->tokens, init_token(mshell, input, 1, PIPE));
		else if (input->full_str[input->index] == '<')
			add_token(&input->tokens, init_token(mshell, input, 1, REDIRECT_IN));
		else if (input->full_str[input->index] == '>')
			add_token(&input->tokens, init_token(mshell, input, 1, REDIRECT_OUT));
		else
			word(mshell, input);
	}
}

int	tokenizer(t_minishell *mshell, t_input *input, char *input_str)
{
	init_lexer(input, input_str);
	create_tokens(mshell, input);
	if (tokens_validation(input->tokens) == FAIL)
		return (FAIL);
	retokenize_words(input->tokens);
	return (SUCCESS);
}
// ls -la<file1>fi"le"1.1| "c"a't' -e >fi""'le2' <<'fi'le3 | grep fi"l"en'am'e >>file4 | du -s > $HOME'/path'
// ls -la<file1>fi"le"1.1| "c"a't' -e >fi""'le2' <<'fi'le3 | grep fi"l"en'am'e >>file4 | du -s > "$HO'ME"'/path'
