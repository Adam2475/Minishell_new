/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:29:58 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 16:03:22 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_isspace(int c)
{
	if (c == 32 || (c >= 7 && c <= 13))
		return (1);
	return (0);
}

int	is_whitespace(const char *str)
{
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = ft_strndup(value, ft_strlen(value));
	new_token->next = NULL;
	return (new_token);
}

void	manual_helper(t_data **data, t_token **tkn,
		char **cmd_args)
{
	if ((*data)->redirect_state == 1)
		dup2((*data)->saved_fd_out, STDOUT_FILENO);
	else if ((*data)->redirect_state == 0)
		dup2((*data)->saved_fd_in, STDIN_FILENO);
	exec_exit2(data, tkn, cmd_args, 0);
}

int	init_state(t_data **data, t_token **tokens, char *tmp)
{
	if (data)
		*tokens = NULL;
	if (!tmp)
		return (1);
	return (0);
}

int	exec_exit4(t_data **data, t_token **tokens,
		char **cmd_args, int print)
{
	print = 0;
	if (cmd_args && print == 0)
		free_char_array(cmd_args);
	if ((*data)->env_p)
		free_char_array((*data)->env_p);
	free_env_list((*data)->env_list);
	free_tokens(data, (*tokens));
	free((*data)->end);
	print = (*data)->local_err_state;
	free((*data));
	exit(print);
}