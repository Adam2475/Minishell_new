/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include <ctype.h>
# include <fcntl.h>
# include <readline/readline.h>
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TRUE 1
# define FALSE 0
# define WHITESPACE 32

# define SINGLE_QUOTES '\''
# define DOUBLE_QUOTES '\"'
# define REDIRECT_LEFT '<'
# define REDIRECT_RIGHT '>'
# define PIPE '|'
# define DOLLAR_SIGN '$'

# define MAX_HISTORY 100
# define MAX_COMMAND_LENGTH 1024

extern int	g_err_state;

typedef struct s_env_list
{
	char				*var;
	char				*value;
	char				*content;
	struct s_env_list	*pre;
	struct s_env_list	*next;
}	t_env_list;

typedef struct s_data
{
	char			*input;
	char			*my_line;
	char			*path_from_envp;
	char			**my_paths;
	t_env_list		*env_list;
}	t_data;

// structural functions
void		free_exit(t_data **data);
void		free_list(t_token *head);
int			init_data(t_data **data, int argc, char **argv, t_token **tokens);
// tokenizer
void		tokenizer(t_data **data, t_token **tokens);
int			whitespace_case(char *buffer, char *end, t_token **tokens);
int			special_cases_lexer(t_data **data, char *buffer,
				t_token **tokens, char *end);
t_token		*token_reformatting_command(t_token *current);
// env_list
t_env_list	*lstlast_env(t_env_list *lst);
void		add_back_env(t_env_list **lst, t_env_list *new);
void		split_var_env(t_env_list **node);
t_env_list	*new_node_env(char *content);
void		gen_list_env(t_data **data, char **envp);
// expander
int			check_quotes(t_token **tokens);
char		*expand_err_state(char *tmp);
int			expand_doll(t_token **current, t_data **data);
int			expand_var(t_token **tkn_lst, t_data **data);
char		*tmp_set(char *val);
// builtins
// chdir
void		chpwd(t_data **data, char *new_path);
int			cd_cmd(t_data **data, t_token **tkn);
// free functions
void		ft_free_null(void *null);

#endif
