/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_doc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	count_doll(char *line)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!line)
		return (0);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]
			&& line[i + 1] != '$')
			count++;
		i++;
	}
	return (count);
}

static	char	*exp_word_2(char *res, t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node && ft_strncmp(res, node->var, ft_strlen(res) - 1) != 0)
	{
		if (node->next)
			node = node->next;
		else if (!node->next)
		{
			node = node->next;
			break ;
		}
	}
	if (!node && *res == '?')
		return (free(res), expand_err_state(res));
	else if (!node)
		return (free(res), ft_strndup("", 1));
	return (free(res), ft_strndup(node->value, ft_strlen(node->value)));
}

char	*exp_word(char *line, t_data **data, int *i)
{
	char		*tmp;
	char		*res;
	int			count;

	count = (*i);
	if (line[(*i)] == '$')
		(*i)++;
	while (line[(*i)] && (ft_isalnum(line[(*i)]) || line[(*i)] == '_'))
		(*i)++;
	count = (*i) - count;
	tmp = (char *)ft_calloc(sizeof(char), count + 1);
	(*i) -= count;
	count = 0;
	if (line[(*i)] == '$')
		(*i)++;
	while (line[(*i)] && (ft_isalnum(line[(*i)]) || line[(*i)] == '_'))
	{
		tmp[count] = line[(*i)];
		count++;
		(*i)++;
	}
	res = tmp_set(tmp);
	free(tmp);
	tmp = exp_word_2(res, data);
	return (tmp);
}

static	char	*exp_line(char *line, char *env, int i)
{
	int		j;
	char	*pre_doll;
	char	*post_doll;

	if (line[i] != '$')
		pre_doll = ft_strndup(line, (ft_strlen_char(line, '$') - 1));
	else
		pre_doll = ft_strndup("", 0);
	j = i;
	while (line[i] != '\0')
		i++;
	post_doll = ft_calloc(sizeof(char), (i - j + 1));
	i = j;
	j = -1;
	while (line[i] != '\0')
		post_doll[++j] = line[i++];
	free(line);
	line = ft_strjoin(pre_doll, env);
	free(pre_doll);
	pre_doll = ft_strjoin(line, post_doll);
	free(line);
	free(post_doll);
	return (pre_doll);
}

char	*expander_doc(char *line, t_data **data)
{
	int		num_doll;
	char	*tmp;
	int		i;

	num_doll = count_doll(line);
	i = 0;
	if (!num_doll)
		return (NULL);
	tmp = NULL;
	while (line[i] && num_doll > 0)
	{
		if (line[i] == '$' && line[i + 1]
			&& line[i + 1] != '$')
		{
			tmp = exp_word(line, data, &i);
			line = exp_line(line, tmp, i);
			i = 0;
			free(tmp);
			num_doll--;
		}
		if (line[i])
			i++;
	}
	return (line);
}
