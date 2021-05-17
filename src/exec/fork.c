/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjung <yjung@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 15:45:57 by yjung             #+#    #+#             */
/*   Updated: 2021/05/17 15:52:28 by yjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pipe_set(t_cmd *cmds, t_check *g)
{
	if (g->pipe_cnt > 0 && ((t_pi_fd *)(g->pipe_fd->content))->check \
		== PIPE_R_ONLY)
		return (ft_built_cmd_set(cmds, g));
	if (g->fd_out > 0)
	{
		if (ft_redir_connect(g, 1) == DUP_ERROR)
			return (DUP_ERROR);
		if (ft_cmd_exec(cmds, g) == FAIL)
			return (FAIL);
		ft_redir_close(g);
		return (SUCCESS);
	}
	return (ft_cmd_exec(cmds, g));
}

int	ft_built_cmd_set(t_cmd *cmds, t_check *g)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		ft_error_print("fail fork", strerror(errno));
	else if (pid == 0)
	{
		ft_pipe_connect(&status, g);
		if (status < 0)
			return (status);
		status = ft_redir_connect(g, 0);
		if (status < 0)
			return (status);
		status = ft_cmd_exec(cmds, g);
		exit(status);
	}
	else
	{
		wait(&pid);
		ft_redir_close(g);
		ft_pipe_close(g);
	}
	return (status);
}

int	ft_cmd_set(t_cmd *cmds, t_check *g)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		ft_error_print("fail fork", strerror(errno));
	if (pid == 0)
	{
		ft_pipe_connect(&status, g);
		if (status < 0)
			return (status);
		status = ft_redir_connect(g, 0);
		if (status < 0)
			return (status);
		status = ft_make_cmd(cmds->cmd, cmds->args);
		exit(status);
	}
	else
	{
		wait(&pid);
		ft_redir_close(g);
		ft_pipe_close(g);
	}
	return (status);
}
