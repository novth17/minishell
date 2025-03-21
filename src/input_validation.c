#include "../inc/minishell.h"

static int	redirection_validation(char *input)
{
	int	i;
	int	in_double;
	int	in_single;

	i = 0;
	in_double = 0;
	in_single = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (input[i] == '"' && !in_single)
			in_double = !in_double;
		else if (!in_single && !in_double)
		{
			if (ft_strnstr(&input[i], "<>", 2) || ft_strnstr(&input[i], "<<<", 3) ||
			ft_strnstr(&input[i], "><", 2) || ft_strnstr(&input[i], ">>>", 3))
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

static int	quote_validation(char *input, t_quotes type, int *balanced)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (type == NONE)
		{
			if (input[i] == '\'')
				i += quote_validation(&input[i + 1], SINGLE, balanced);
			if (input[i] == '"')
				i += quote_validation(&input[i + 1], DOUBLE, balanced);
		}
		else if (type == SINGLE || type == DOUBLE)
		{
			if (input[i] == '\'' && type == SINGLE)
				return (i + 1);
			else if (input[i] == '"' && type == DOUBLE)
				return (i + 1);
		}
		i++;
	}
	if (type == SINGLE || type == DOUBLE)
		*balanced = 0;
	return (i);
}

int input_validation(char *input)
{
	int	balanced;

	balanced = 1;
	if (!input)
		return(FAIL);
	quote_validation(input, NONE, &balanced);
	if(!balanced)
	{
		print_error(BALANCE, NULL);
		return (FAIL);
	}
	if (redirection_validation(input))
	{
		print_error(REDIRECT, NULL);
		return (FAIL);
	}
	return (SUCCESS);
}
