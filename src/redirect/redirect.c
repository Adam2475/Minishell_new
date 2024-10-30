/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/30 19:16:56 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	cmd_args_filler(t_data **data, t_token **tokens,
	char **cmd_args, t_token **tkn)
{
	(*data)->tmp6 = (*data)->tmp90;
	(*data)->command2 = (*data)->cmd42;
	(*data)->cmd_args_exit = cmd_args;
	if (manual_cmd(cmd_args, data, tokens))
	{
		free((*data)->tmp90);
		(*data)->tmp90 = NULL;
		free((*data)->cmd42);
		(*data)->cmd42 = NULL;
		manual_helper(data, tkn, cmd_args);
	}
}

static	void	execution_init_helper(t_data **data, t_token **current,
	t_token **tokens)
{
	t_token	*head;

	head = *tokens;
	*current = *tokens;
	(*data)->tmp90 = NULL;
	while ((*current)->type != TOKEN_EOF)
	{
		if ((*current)->type == TOKEN_COMMAND)
			(*data)->tmp90 = ft_strdup((*current)->value);
		*current = (*current)->next;
	}
	*current = head;
	command_extractor(data, *current);
}

static	char	**execution_helper3(t_data **data)
{
	char	**cmd_args;

	if ((*data)->tmp90 && (*data)->tmp90 != NULL)
	{
		cmd_args = ft_split((*data)->tmp90, '\n');
		(*data)->cmd42 = ft_strdup(cmd_args[0]);
	}
	else
	{
		(*data)->cmd42 = NULL;
		cmd_args = NULL;
	}
	if ((*data)->tmp90 && !cmd_args)
		cmd_args[0] = ft_strdup((*data)->tmp90);
	free((*data)->command2);
	(*data)->tmp6 = NULL;
	return (cmd_args);
}

static	void	execution_helper4(t_data **data, char **cmd_args,
	t_token *tokens, t_token *tkn)
{
	if (cmd_args)
		cmd_args_filler(data, &tokens, cmd_args, &tkn);
	if ((*data)->cmd42)
		(*data)->holder = find_cmd((*data)->cmd42, data);
	else
		(*data)->holder = NULL;
}

int	execute_command(t_data **data, char **envp, t_token **tkn, t_token **tokens)
{
	char	**cmd_args;
	t_token	*current;

	execution_init_helper(data, &current, tokens);
	cmd_args = execution_helper3(data);
	execution_helper4(data, cmd_args, *tokens, *tkn);
	if ((*data)->cmd42 && !(*data)->holder)
	{
		if (cmd_args)
			free_char_array(cmd_args);
		cmd_args = ft_calloc(sizeof(char **), 1);
		(*data)->holder = ft_strndup((*data)->cmd42, ft_strlen((*data)->cmd42));
	}
	if ((*data)->cmd42)
		free((*data)->cmd42);
	if (cmd_args)
		execve((*data)->holder, cmd_args, envp);
	usleep(30000);
	if (check_token_type(*tokens) < 0)
		(*data)->local_err_state = 127;
	cleanup_helper(data, cmd_args, *tkn);
	return (0);
}
