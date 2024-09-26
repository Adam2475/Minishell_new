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

// t_token	*copy_till_pipe(t_data **data)
// {
// 	t_token	*tmp;
// 	t_token	*head;
// 	t_token	*tmp_head;
// 	t_token	*prev;

// 	tmp_head = (*data)->tmp;
// 	head = tmp;
// 	while ((*data)->tmp->type != TOKEN_PIPE)
// 	{
// 		prev = (*data)->tmp;
// 		//tmp = copy_token((*data)->tmp);
// 		ft_tokenadd_back(&tmp, prev);
// 		(*data)->tmp = (*data)->tmp->next;
// 		free(prev);
// 	}
// 	tmp = head;
// 	(*data)->tmp = tmp_head;
// 	return (tmp);
// }

// static	t_token	*copy_split()
// {

// }

// t_token *copy_until_pipe(t_token *src)
// {
//     if (!src) return NULL;

//     t_token *new_list = NULL;
//     t_token *current_new = NULL;

//     while (src && src->type != TOKEN_PIPE) {
//         t_token_list *new_node = malloc(sizeof(t_token_list));
//         if (!new_node) {
//             // Handle memory cleanup in case of allocation failure
//             // You can add a cleanup function here if needed
//             return NULL;
//         }

//         new_node->token = copy_token(src);
//         if (!new_node->token) {
//             free(new_node);
//             return NULL;
//         }

//         new_node->next = NULL;

//         if (!new_list) {
//             new_list = new_node;
//         } else {
//             current_new->next = new_node;
//         }
//         current_new = new_node;

//         src = src->next;
//     }

//     return new_list;
// }

static	void	split_tokens(t_data **data, t_token *src)
{
	t_token	*head1;
	t_token *head2;
	t_token *end;
	head1 = src;
	int i;
	i = 0;
	t_token_list *result;
	t_token_list *result_head;
	int count = count_pipes(src) + 1;
	result = (t_token_list *)ft_calloc(sizeof(t_token_list), count + 1);
	//ft_printf("%s\n", src->value);
	result_head = result;
	while (i < count && src != NULL)
	{
		ft_tokenadd_back(&result->head, ft_lstnewtoken(src->type, ft_strdup(src->value)));
		while (src->type != TOKEN_PIPE && src->type != TOKEN_EOF)
		{
			src = src->next;
			ft_tokenadd_back(&result->head, ft_lstnewtoken(src->type, ft_strdup(src->value)));
		}
		i++;
		if (src->type == TOKEN_PIPE)
		{
			result = result->next;
		}
		src = src->next;
	}
	result = result_head;
	//free_token_list(result);
	(*data)->token_list = result;
	src = head1;
	return;
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;
	t_token		*tokens;
	t_token		*tmp;
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
		//data->tmp = copy_token_list(&data, tokens);
		data->tokens = copy_token_list(&data, tokens);
		env_parser(&data, envp);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
		{
			tmp = copy_token_list(&data, data->tokens);
			//data->token_list = split_tokens_by_pipe(data->tmp);
			//data->token_list = split_tokens_by_pipe(tmp);
			split_tokens(&data, tmp);
			//if (data->tmp != NULL)
			free_list(data->tmp);
			print_token_lists(data->token_list);
			pipe_case(&tokens, &data, envp, &data->token_list);
		}
		//print_tokens(tokens);
		free_list(tmp);
		//free_list(data->tmp);
		free_tokens(&data, tokens);
	}
}

// Testing

////////////////
// Edge Cases:
// diomerda
// = current_list->head;

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
// export a=32 b=78 c=4647 ?! | leaks
// echo cioa$PWD ciao

/////////////////
// Multi Cmd:
//
// < outfile grep -rl ada | cat -e > out2 ?!
// < src/init.c grep -rl int | cat -e > out2 ?!
// cat src/main.c | cat src/init.c ?!
