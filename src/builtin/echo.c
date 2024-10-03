/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/03 17:12:32 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	heredoc_unlink(t_data **data)
{
	if ((*data)->heredoc_flag > 0)
	{
		(*data)->heredoc_flag = 0;
		unlink(".heredoc.txt");
	}
}

void	free_tokens(t_data **data, t_token *tokens)
{
	if (tokens)
		free_list(tokens);
	heredoc_unlink(data);
	if ((*data)->tokens)
		free_list((*data)->tokens);
	if ((*data)->token_list != NULL)
		free_token_list((*data)->token_list);
	if ((*data)->fd >= 0)
		(*data)->fd = -1;
	if ((*data)->path_from_envp)
		free((*data)->path_from_envp);
	if ((*data)->command)
		free_char_array((*data)->command);
	if ((*data)->my_paths)
		free_char_array((*data)->my_paths);
	if ((*data)->my_line)
		free((*data)->my_line);
	if ((*data)->cmd2)
	{
		free((*data)->cmd2);
		(*data)->cmd2 = NULL;
	}
	if ((*data)->cmd_args)
	{
		free_char_array((*data)->cmd_args);
		(*data)->cmd_args = NULL;
	}
	free((*data)->input);
}

static	void	print_in_qt(t_token **node, t_token_type type)
{
	(*node) = (*node)->next;
	while ((*node)->type != type && (*node)->type != TOKEN_EOF)
	{
		ft_printf("%s", (*node)->value);
		(*node) = (*node)->next;
	}
	if ((int)(*node)->next->type == 11 && (*node)->type == type)
		ft_printf(" ");
}

static void	inutils_num(t_token *tmp, int *flag_n)
{
	char	*n_s;

	n_s = tmp->value;
	while (tmp && tmp->type != 7)
	{
		if (ft_strncmp(n_s, "-n", 1) == 0)
			n_s++;
		if (*n_s && *n_s == 'n')
		{
			n_s++;
			continue ;
		}
		else if (!*n_s || *n_s == '\0')
		{
			tmp->type = TOKEN_OPTION;
			tmp = tmp->next;
			while (tmp->type != 7 && tmp->type == 11)
				tmp = tmp->next;
			n_s = tmp->value;
			(*flag_n) += 1;
		}
		else if (*n_s && *n_s != '\0' && *n_s != 'n')
			break ;
	}
	tmp->type = TOKEN_APPENDICE;
}

int	echo_cmd(t_token **tkn)
{
	t_token	*node;
	int		flag_n;

	node = (*tkn)->next;
	flag_n = 0;
	while (node->type != TOKEN_EOF && node->type == TOKEN_WHITESPACE)
		node = node->next;
	inutils_num(node, &flag_n);
	node = (*tkn)->next;
	while (node && node->type != TOKEN_EOF)
	{
		if (((int)node->type == 8 || (int)node->type == 13)
			&& (int)node->next->type == 11)
			ft_printf("%s ", node->value);
		if (((int)node->type == 8 || (int)node->type == 13)
			&& (int)node->next->type != 11)
			ft_printf("%s", node->value);
		if ((int)node->type == 9 || (int)node->type == 10)
			print_in_qt(&node, node->type);
		node = node->next;
	}
	if (flag_n == 0)
		ft_printf("\n");
	return (g_err_state = 0, 1);
}
