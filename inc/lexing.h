#ifndef LEXING_H
# define LEXING_H

typedef enum e_quotes
{
	NONE,
	SINGLE,
	DOUBLE,
}	t_quotes;

typedef enum e_token_type
{
	WORD = 1,			//1 some word in quotes or without quotes
	COMMAND,			//2 word classified as command
	ARG,				//3 word classified as argument to command
	FILE_TOKEN,			//4 word classified as file
	HERE_TOKEN,			//5 word classified as heredoc
	PIPE,				//6 |
	REDIRECT_IN,		//7 <
	REDIRECT_OUT,		//8 >
	REDIRECT_APPEND,	//9 >>
	HERE_DOCUMENT,		//10 <<
	ENV_VAR,			//11 $
}	t_token_type;

typedef struct s_token
{
	char			*value;
	int				len;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct	s_input
{
	char	*full_str;	// The full user input
	int		index;		// track position while scanning
	int		len;		// len helps check if end of input is reached
	t_token	*tokens;
}				t_input;

typedef struct s_quotes_helper
{
	int	in_double;
	int	in_single;
	int	in_quotes;
	int	previous_in_quotes;
}				t_quotes_helper;

#endif
