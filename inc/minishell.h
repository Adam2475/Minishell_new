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

typedef enum cmd
{
	NONE,
	CH_DIR,
	ECHO,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	PWD
}	t_cmd;

typedef struct s_token_list
{
	t_token		*head;
	struct s_token_list *next;
}	t_token_list;

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
	char			**command;
	char			**cmd_args;
	char			*cmd;
	int				redirect_state;
	int				fd;
	t_token			*tmp;
	t_token			*tokens;
	t_cmd			cmd;
	t_token_list	*token_list;
	t_env_list		*env_list;
}	t_data;

// Structural functions
void		free_exit(t_data **data, t_token *tokens);
void		exit_from_parser(t_data **data, t_token *tokens);
void		free_list(t_token *head);
int			init_data(t_data **data, int argc, char **argv, t_token **tokens);
// Tokenizer
void		tokenizer(t_data **data, t_token **tokens);
int			whitespace_case(char *buffer, char *end, t_token **tokens);
int			special_cases_lexer(t_data **data, char *buffer,
			t_token **tokens, char *end);
t_token		*token_reformatting_command(t_token *current);
t_token		*copy_token_list(t_token *tokens);
t_token		*token_reformatting_pipe(t_token *current);
t_token		*token_reformatting_special(t_token *current);
// Parser
int			piper(t_token **tokens);
int			token_parser(t_token **tokens, t_data **data, char **envp);
int			parser_case_redo(t_token *current, t_data **data);
int			parser_case_redi(t_token *current, t_data **data);
int			parser_case_append(t_token *current, t_data **data);
int			parser_case_herdoc(t_token *current, t_data **data);
// Redireciton
void		handle_heredoc(char *delimiter, t_data **data);
// Executer
void	execute_command_single(char **command, t_data **data, char **envp, t_token **token);
// builtins
char		*find_cmd(char *cmd, t_data **data);
void		free_char_array(char **array);

#endif
