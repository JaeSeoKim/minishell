/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjung <yjung@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 21:11:40 by yjung             #+#    #+#             */
/*   Updated: 2021/05/17 18:00:35 by yjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error(char *env)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(env, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static void	print_export(void)
{
	t_list	*env;
	char	*ptr;
	char	*str;

	env = g_sh.envp;
	while (env)
	{
		ptr = ft_strchr(env->content, '=');
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		if (ptr)
		{
			str = ft_strndup(env->content, (ptr - (char *)env->content) + 1);
			ft_putstr_fd(str, STDOUT_FILENO);
			free(str);
			ft_putstr_fd("\"", STDOUT_FILENO);
			str = ft_strndup(ptr + 1, ft_strlen(ptr + 1));
			ft_putstr_fd(str, STDOUT_FILENO);
			free(str);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		else
			ft_putstr_fd(env->content, STDOUT_FILENO);
		env = env->next;
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

static int	check_envp_name(char *arg)
{
	char	*ptr;
	int		len;

	ptr = ft_strchr(arg, '=');
	if (ptr)
		len = ptr - arg;
	else
		len = ft_strlen(arg);
	while (--len >= 0)
	{
		if ((len == 0 && !(ft_isalpha(arg[len]) || arg[len] == '_')) \
			|| !(ft_isalnum(arg[len]) || arg[len] == '_'))
			return (FAIL);
	}
	return (SUCCESS);
}

int	ft_export(t_list *args)
{
	char	*value;

	if (!args)
		print_export();
	while (args)
	{
		value = ft_strdup(args->content);
		if (value == NULL)
			return (MALLOC_FAIL);
		if (check_envp_name(value) == FAIL)
		{
			print_error(value);
			ft_free(value);
		}
		else if (set_envp(value) == MALLOC_FAIL)
			return (MALLOC_FAIL);
		args = args->next;
	}
	return (SUCCESS);
}
