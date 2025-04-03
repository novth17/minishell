
#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdio.h>

typedef struct s_env t_env;
typedef struct s_ast t_ast;
typedef struct s_minishell t_minishell;

int count_cmds(char **cmds);
int		execute_builtins(t_minishell *mshell, t_ast *ast);
int		ft_env(t_env **my_env, char **args);
int		ft_pwd(void);
int		ft_echo(int argc, char **argv);
void	ft_exit(char **args, t_minishell *minishell);
int		ft_unset(char **args, t_env *env);

//int ft_cd(t_env *env, char **args);
//int execute_builtin(t_minishell *mshell, t_ast_test *ast);

#endif
