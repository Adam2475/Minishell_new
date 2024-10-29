/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 16:04:44 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	cmd_args_filler(t_data **data, t_token **tokens,
	char *tmp, char *cmd, char **cmd_args, t_token **tkn)
{
	(*data)->tmp6 = tmp;
	(*data)->command2 = cmd;
	(*data)->cmd_args_exit = cmd_args;
	if (manual_cmd(cmd_args, data, tokens))
	{
		free(tmp);
		tmp = NULL;
		free(cmd);
		cmd = NULL;
		manual_helper(data, tkn, cmd_args);
	}
}

static	void	execution_init_helper(t_data **data, t_token **current,
	t_token **tokens, char **tmp)
{
	t_token *head;

	head = *tokens;
	*current = *tokens;
	*tmp = NULL;
	while ((*current)->type != TOKEN_EOF)
	{
		if ((*current)->type == TOKEN_COMMAND)
			*tmp = ft_strdup((*current)->value);
		*current = (*current)->next;
	}
	*current = head;
	command_extractor(data, *current, tmp);
}

int	execute_command(t_data **data, char **envp, t_token **tkn, t_token **tokens)
{
	char	*cmd;
	char	**cmd_args;
	char	*tmp;
	t_token	*current;

	execution_init_helper(data, &current, tokens, &tmp);
	if (tmp && tmp != NULL)
	{
		cmd_args = ft_split(tmp, 32);
		cmd = ft_strdup(cmd_args[0]);
	}
	else
	{
		cmd = NULL;
		cmd_args = NULL;
	}
	if (tmp && !cmd_args)
		cmd_args[0] = ft_strdup(tmp);
	free((*data)->command2);
	(*data)->tmp6 = NULL;
	if (cmd_args)
		cmd_args_filler(data, tokens, tmp, cmd, cmd_args, tkn);
	if (cmd)
		(*data)->holder = find_cmd(cmd, data);
	else
		(*data)->holder = NULL;
	if (cmd && !(*data)->holder)
	{
		if (cmd_args)
			free_char_array(cmd_args);
		cmd_args = ft_calloc(sizeof(char **), 1);
		(*data)->holder = ft_strndup(cmd, ft_strlen(cmd));
	}
	if (cmd)
		free(cmd);
	if (cmd_args)
		execve((*data)->holder, cmd_args, envp);
	cleanup_helper(data, &tmp, cmd_args, *tkn);
	return (0);
}
