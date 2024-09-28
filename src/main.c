/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/25 18:20:23 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_err_state;

void	print_tokens(t_token *tokens)
{
	t_token *temp = tokens;
		while (temp)
	{
		printf("Type: %d, Value: %s\n", temp->type, temp->value);
		temp = temp->next;
	}
}

static	char	*retrieve_line(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			return (ft_strndup(envp[i], ft_strlen(envp[i])));
		i++;
	}
	return (NULL);
}

static	void	env_parser(t_data **data, char **envp)
{
	if ((*data)->env_list == NULL)
		gen_list_env(data, envp);
	(*data)->my_line = retrieve_line(envp);
	if (!(*data)->my_line)
		exit(write(1, "PATH not found\n", 15));
	(*data)->path_from_envp = ft_substr((*data)->my_line, 5, 500);
	(*data)->my_paths = ft_split((*data)->path_from_envp, ':');
}

static void	add_tokens_to_list(t_token_list *result, t_token **src)
{
	while ((*src)->type != TOKEN_PIPE && (*src)->type != TOKEN_EOF)
	{
		ft_tokenadd_back(&result->head,
			ft_lstnewtoken((*src)->type, ft_strdup((*src)->value)));
		*src = (*src)->next;
	}
}

static void	split_tokens(t_data **data, t_token *src)
{
	t_token_list	*result;
	t_token_list	*result_head;
	int				i;
	int				count;

	i = 0;
	count = count_pipes(src) + 1;
	result = (t_token_list *)ft_calloc(1, sizeof(t_token_list));
	result_head = result;
	while (i < count && src != NULL)
	{
		add_tokens_to_list(result, &src);
		if (src->type == TOKEN_PIPE)
		{
			src = src->next;
			if (src != NULL && i++ < count)
			{
				result->next = (t_token_list *)ft_calloc(1,
						sizeof(t_token_list));
				result = result->next;
			}
		}
	}
	result->next = NULL;
	(*data)->token_list = result_head;
}

static	int	read_input(t_data *data, t_token *tokens)
{
	data->token_list = NULL;
	data->input = readline("myprompt$ ");
	if (!data->input)
		return (0);
	add_history(data->input);
	data->tokens = copy_token_list(&data, tokens);
	return (1);
}

static	void	do_pipe(t_data *data, t_token *tokens, char **envp)
{
	t_token	*tmp;

	tmp = copy_token_list(&data, data->tokens);
	split_tokens(&data, tmp);
	free_list(data->tmp);
	print_token_lists(data->token_list);
	pipe_case(&tokens, &data, envp, &data->token_list);
	free_list(tmp);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;
	t_token		*tokens;
	int			flags;

	if (init_data(&data, argc, argv, &tokens) > 0)
		return (1);
	flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	if (!envp)
		return (1);
	gen_list_env(&data, envp);
	set_signal();
	while (1)
	{
		if (!read_input(data, tokens))
			return (ft_printf("exit\n"), free_exit(&data), 1);
		if (tokenizer(&data, &tokens))
			continue ;
		env_parser(&data, envp);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
			do_pipe(data, tokens, envp);
		free_tokens(&data, tokens);
	}
}

// Testing

////////////////
// Edge Cases:
// diomerda
// = current_list->head; | OK
// ljsdbhhds hdsdsh  > | lhsdb<dshh !?

/////////////////
// Single Command:
// echo ciao | OK
// echo "ciao" ciao | OK
// ls -l | OK
// exit
// ls -l > outfile
// cat outfile
// < outfile grep -rl out
// cat << eof | OK
// ls -l >> out | OK
// export a=32 b=78 c=4647 | OK
// echo cioa$PWD ciao | OK

/////////////////
// Multi Cmd:
//
// < outfile grep -rl ada | cat -e > out2 | OK
// < src/init.c grep -rl int | cat -e > out2 | OK
// cat src/main.c | cat src/init.c | OK
// env | sort | grep -v SHLVL | grep -v ^_ | OK

// multi cmd still have issues when run as second command
