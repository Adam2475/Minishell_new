/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/08 18:21:56 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_err_state;

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
	while (i++ < count && src != NULL)
	{
		add_tokens_to_list(result, &src);
		if (src->type == TOKEN_PIPE)
		{
			src = src->next;
			if (src != NULL)
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

static	int	read_input(t_data *data)
{
	data->token_list = NULL;
	data->tokens = NULL;
	data->merdoso = 0;
	data->input = readline("myprompt$ ");
	if (!data->input)
		return (errno = 126, 0);
	if (data->input[0] != '\0')
		add_history(data->input);
	return (1);
}

void	do_pipe(t_data *data, t_token *tokens, char **envp)
{
	t_token	*tmp;

	tmp = copy_token_list(&data, tokens);
	split_tokens(&data, tmp);
	free_list(data->tmp);
	free_list(tmp);
	pipe_case(&tokens, &data, envp, &data->token_list);
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
		if (tokens)
			free_tokens(&data, tokens);
		if (!read_input(data))
			return (ft_printf("exit\n"), free_exit(&data), 1);
		if (data->input[0] == '\0' || tokenizer(&data, &tokens))
			continue ;
		env_parser(&data, envp);
			// continue ;
		command_init(data, tokens, envp);
	}
}

// Edge Cases:
// diomerda | OK
// = current_list->head; | OK
// ljsdbhhds hdsdsh > | lhsdb<dshh !? (leaks)
// t_token *result; = NULL; | OK
// "/usr/bin/ls" | OK
// "           "  (only as second command) | OK

// Single Command:
// echo ciao | OK
// echo -nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn ciao OK
// echo -n -n -n -n -n ciao OK
// echo -n -n -nf -n ciao OK
// echo "ciao" ciao | OK
// ec"ho" ciao | KO
// ls -l | OK
// exit | OK
// ls -l > outfile OK
// cat outfile OK
// < outfile grep -rl out OK
// cat << eof | OK
// a << s << z << x OK
// ls -l >> out | OK
// export a=32 b=78 c=4647 | OK
// echo cioa$PWD ciao | OK

// Multi Cmd:
//
// cat merda | cat ciao | OK
// < outfile grep -rl ada | cat -e > out2 | OK
// < src/init.c grep -rl int | cat -e > out2 | OK
// cat src/main.c | cat src/init.c | OK
// env | sort | grep -v SHLVL | grep -v ^_ | OK
// < out env | sort | grep -v SHLVL | grep -v ^_ | OK
// out < env | sort | grep -v SHLVL | grep -v ^_ | segfault

// multi cmd still have issues when run as second command ?!
// unset home e cd senza argomenti
// cat << << eof

// unset $PATH
// program should not work
// absolute command
// env | sort | grep -v SHLVL | grep -v ^_

// TODO : add lexical control to export 
// TODO : add getcwd to make pwd work with unset || OK
// TODO : comand env && builtins usage with >> & <<