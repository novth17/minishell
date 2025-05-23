/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:03:49 by hiennguy          #+#    #+#             */
/*   Updated: 2025/04/25 21:04:19 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_env_var(t_env *env, const char *key);

int	ft_unset(char **args, t_env *env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_var(env, args[i]);
		i++;
	}
	return (SUCCESS);
}

static void	remove_env_var(t_env *env, const char *key)
{
	unsigned int	cur_index;
	unsigned int	shift_left;

	cur_index = 0;
	while (cur_index < env->len)
	{
		if (match_env_key(env->envp[cur_index], key))
		{
			free(env->envp[cur_index]);
			shift_left = cur_index;
			while (shift_left < env->len - 1)
			{
				env->envp[shift_left] = env->envp[shift_left + 1];
				shift_left++;
			}
			env->envp[env->len - 1] = NULL;
			env->len--;
			return ;
		}
		cur_index++;
	}
}
