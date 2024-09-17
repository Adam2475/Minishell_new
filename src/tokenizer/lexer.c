/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	recognizer(char *buffer, t_token *tokens,
		char *end, t_data **data)
{
	while (*buffer)
	{
		if (*buffer == WHITESPACE)
		{
			*buffer += whitespace_case(buffer, end,  &tokens);
			continue ;
		}
		if (*buffer == REDIRECT_LEFT || *buffer == REDIRECT_RIGHT ||
				*buffer == PIPE || *buffer == '$' || *buffer == SINGLE_QUOTES || *buffer == DOUBLE_QUOTES)
		{
			buffer += special_cases_lexer(data, buffer, &tokens);
			continue;
		}
		end = buffer;
		while (*end && *end != WHITESPACE && *end != REDIRECT_LEFT &&
				*end != PIPE && *end != REDIRECT_RIGHT && *end != '$' && *end != DOUBLE_QUOTES && *end != SINGLE_QUOTES)
			end++;
		if (*buffer == '-')
			ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_OPTION, ft_strndup(buffer, end - buffer)));
		else
			ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_WORD, ft_strndup(buffer, end - buffer)));
		buffer = end;
	}
}

static	int	init_state(t_data **data, t_token **tokens, char *tmp)
{
	if (data)
		*tokens = NULL;
	if (!tmp)
		return (1);
	return (0);
}

void	tokenizer(t_data **data, t_token **tokens)
{
	char	*buffer;
	char	*tmp;
	char	*end;

	end = NULL;
	tmp = ft_strndup((*data)->input, ft_strlen((*data)->input));
	if (init_state(data, tokens, tmp) > 0)
	{
		free(tmp);
		free_exit(data);
	}
	buffer = tmp;
	end = buffer;
	recognizer(buffer, *tokens, end, data);
	free (tmp);
}
