#include "../inc/minishell.h"

int	is_pid_expansion(t_quotes_helper quotes, char *input_str)
{
	size_t	i;

	i = 0;
	return ((!quotes.in_single && input_str[i] == '$' && input_str[i + 1] == '$'));
}

int	is_exitcode_expansion(t_quotes_helper quotes, char *input_str)
{
	size_t	i;

	i = 0;
	return ((!quotes.in_single && input_str[i] == '$' && input_str[i + 1] == '?'));
}
