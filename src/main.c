/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/23 18:26:19 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_err_state;

static	void	print_tokens(t_token *tokens)
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
		data->token_list = NULL;
		data->input = readline("myprompt$ ");
		if (!data->input)
			return (ft_printf("exit\n"), free_exit(&data, tokens), 1);
		add_history(data->input);
		if (tokenizer(&data, &tokens))
			continue ;
		data->tmp = copy_token_list(&data, tokens);
		data->tokens = copy_token_list(&data, tokens);
		env_parser(&data, envp);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
		{
			data->token_list = split_tokens_by_pipe(data->tmp);
			pipe_case(&tokens, &data, envp, &data->token_list);
		}
		print_tokens(tokens);
		free_tokens(&data, tokens);
	}
}

// Testing

////////////////
// Edge Cases:
// diomerda

/////////////////
// Single Command:
// echo ciao
// echo "ciao" ciao
// ls -l
// exit
// ls -l > outfile
// cat outfile
// < outfile grep -rl out
// cat << eof
// ls -l >> out

/////////////////
// Multi Cmd:
//
// < outfile grep -rl ada | cat -e > out2 ??
// < src/init.c grep -rl int | cat -e > out2

