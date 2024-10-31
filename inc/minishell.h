/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 13:44:20 by mapichec         ###   ########.fr       */
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
# include <sys/ioctl.h>

# define TRUE 1
# define FALSE 0
# define WHITESPACE 32
# define PATH_MAX 4096

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
	t_token						*head;
	struct s_token_list			*next;
}	t_token_list;

typedef struct s_env_list
{
	char				*var;
	char				*value;
	char				*content;
	int					state;
	struct s_env_list	*pre;
	struct s_env_list	*next;
}	t_env_list;

typedef struct s_data
{
	int				redirect_state;
	int				fd;
	int				total;
	char			*input;
	char			*my_line;
	char			*path_from_envp;
	char			**my_paths;
	char			**command;
	char			**cmd_args;
	char			**cmd_args_exit;
	char			**env_p;
	char			*cmd2;
	char			*tmp9;
	char			*tmp6;
	int				*end;
	int				command_found;
	int				pipes;
	int				prev_fd;
	int				merdoso;
	int				in_tmp;
	int				heredoc_flag;
	int				saved_fd;
	int				fd_in;
	int				fd_out;
	int				redirect_state_in;
	int				redirect_state_out;
	int				fd_tmp;
	int				saved_fd_out;
	int				saved_fd_in;
	int				hd_flag;
	int				status;
	int				skip_flag;
	pid_t			*parent;
	int				local_err_state;
	t_token			*tokens_ptr;
	char			*command2;
	char			*holder;
	char			*cmd42;
	int				counter;
	char			*tmp90;
	char			**cmd_args2;
	t_token			*new_token;
	t_token			*tmp;
	t_token			*tokens;
	t_cmd			cmd;
	t_token_list	*token_list;
	t_env_list		*env_list;
}	t_data;

// Structural functions
void			free_exit(t_data **data);
void			free_list(t_token *head);
int				init_data(t_data **data, int argc, char **argv,
					t_token **tokens);
void			command_extractor(t_data **data, t_token *current);
void			cleanup_helper(t_data **data,
					char **cmd_args, t_token *tkn);
// Tokenizer
int				tokenizer(t_data **data, t_token **tokens);
int				whitespace_case(char *buffer, char *end, t_token **tokens);
int				special_cases_lexer(t_data **data, char *buffer,
					t_token **tokens, char *end);
t_token			*token_reformatting_command(t_token *current);
t_token			*copy_token_list(t_data **data, t_token *tokens);
t_token			*token_reformatting_pipe(t_token *current);
t_token			*token_reformatting_special(t_token *current);
int				check_token_type(t_token *head);
int				check_spaces(t_token *tokens);
void			skip_tkn_space(t_token **current);
// Parser
int				piper(t_token **tokens);
int				token_parser(t_token **tokens, t_data **data);
int				parser_case_redo(t_token *current, t_data **data);
int				parser_case_redi(t_token *current, t_data **data);
int				parser_case_append(t_token *current, t_data **data);
int				parser_case_herdoc(t_token *current, t_data **data,
					t_token **tokens);
int				redirect_parser(t_data **data, t_token *current,
					t_token **tokens);
int				command_reformatter(t_token *token_list);
// Redireciton
char			*expander_doc(char *line, t_data **data);
char			*exp_word(char *line, t_data **data, int *i);
int				handle_heredoc(char *delimiter, t_data **data);
// Executer
void			execute_command_single(char **command, t_data **data,
					t_token **token);
int				copy_mtx2_pt2(t_data **data, int i);
void			copy_mtx2(t_data **data);
int				copy_mtx1_pt2(t_data **data, int i);
int				copy_mtx1(t_data **data);
void			cmd_in_qt(t_token *current);
int				call_doll(t_token **current, int *flag);
void			doll_to_cmd(t_token **tkn);
void			join_in_qt_tk(t_token *tkn, t_token_type type);
int				shrink_tkn_in_qt(t_token **tokens);
void			add_tokens_to_list(t_token_list *result, t_token **src);
int				is_only_spaces(const char *str);
void			split_tokens(t_data **data, t_token *src);
int				read_input(t_data *data);

// env_list
t_env_list		*lstlast_env(t_env_list *lst);
t_env_list		*new_node_env(char *content);
void			add_back_env(t_env_list **lst, t_env_list *new);
void			split_var_env(t_env_list **node);
int				gen_list_env(t_data **data, char **envp);
// signals
void			set_signal(void);
// expander
int				case_err(t_token **current, char *tmp, t_data **data);
int				ft_isalpha_len2(char *str);
int				expand_doll_2(t_env_list *node, t_token **current,
					char **tmp, int len);
t_token			*ft_set_zero(t_token *current, int flag);
void			expand_doll_3(t_token **current, char **tmp, char **tmp2);
char			*expand_err_state(char *tmp, t_data **data);
char			*tmp_set(char *val);
int				check_quotes(t_token **tokens);
int				expand_doll(t_token **current, t_data **data);
int				expand_var(t_token **tkn_lst, t_data **data);
// builtins
char			*find_cmd(char *cmd, t_data **data);
int				manual_cmd(char **cmd_args, t_data **data, t_token **token);
// unset
int				unset_env(t_token **token, t_env_list **env, t_data **data);
int				ft_strsearch(char *str, int c);
// export
int				util_exp(t_data **data, t_token **current, t_token **tkn);
int				inutil_exp(t_data **data, t_token **current, t_token **tkn);
void			print_exp_env(t_data **data);
int				check_for_flag(t_token **tkn);
int				conf_man_cmd(char *str);
void			ft_remove_ws(t_token **token);
void			join_in_qt_exp(t_token *tkn);
int				export_cmd(t_data **data, t_token **tkn);
int				add_to_env(t_token *arg, t_data **data);
int				join_to_env(t_token *arg, t_data **data);
int				is_numeric(char *str);
void			clean_qt(t_token **tkn);
int				compare_path(char *str);
int				ft_isalpha_plus(char *str);
void			add_for_exp(t_data **data, char *arg);
int				inutil_exp2(t_token **current, t_data **data);
// chdir
int				cd_cmd(t_data **data, t_token **tkn);
int				cd_minus(t_token *current, t_data **data);
int				cd_tilde(t_token *current, t_data **data);
int				cd_null(t_token *current, t_data **data);
// echo
int				echo_cmd(t_token **tkn, t_data **data);
void			print_echo(t_token **node, int *flag_2);
// pwd
int				pwd_cmd(t_data **data);
// env
int				env_cmd(t_data **data, t_token **token);
// exit
int				cmd_exit(t_data **data, t_token **token);
int				ft_too_long(char *val, t_data **data);
int				init_execution(t_data **data, int *i);
void			*ft_check_lon(char *s);
void			free_exit_cmd(t_data **data, t_token *tokens);
// free functions
void			ft_free_null(void *null);
void			free_node_env(t_env_list *node);
void			free_char_array(char **array);
void			free_env_list(t_env_list *head);
void			free_tokens(t_data **data, t_token *tokens);
// free functions
void			ft_free_null(void *null);
void			free_char_array(char **array);
void			free_env_list(t_env_list *head);
void			free_tokens(t_data **data, t_token *tokens);
void			skip_space_qt(t_token **current);
// Pipe case
t_token_list	*split_tokens_by_pipe(t_token *tokens);
t_token_list	*create_token_list_node(t_token *head);
t_token			*extract_command_and_appendices(t_data **data,
					t_token *tokens);
size_t			calculate_command_length(t_token *head);
void			append_token(t_token **list, t_token *new_token);
int				pipe_case(t_token **tokens, t_data **data,
					t_token_list **token_list);
void			append_token_list(t_token_list **list, t_token *head);
char			*token_to_command(t_token *head);
int				count_pipes(t_token *head);
int				execute_command(t_data **data, char **envp,
					t_token **tkn, t_token **tokens);
void			space_helper(t_token **head, t_token **current,
					t_token **prev, int flag);
char			*trim_whitespace(char *str);
char			*trim_quotes(char *str);
void			free_tokens_helper(t_data **data);

///////////////////////////////////////////////////////////////////
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strchr(const char *string, int searchedChar );
void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t elementCount, size_t elementSize);
char			*ft_free(char *buffer, char *buf);
char			*ft_next(char *buffer);
char			*ft_line(char *buffer);
char			*read_file(int fd, char *res);
char			*get_next_line2(int fd);
void			free_token(t_token *token);
void			do_pipe(t_data *data, t_token *tokens);
////////////////////////////////////////////////////////////////////
void			print_tokens(t_token *tokens);
int				helper_function2(t_token **current,
					t_token_type type, t_token **tkn);

////////////////////////////////////////////////////////////////////
int				is_whitespace(const char *str);
void			remove_whitespace_nodes(t_token **head);
void			heredoc_unlink(t_data **data);
t_token			*new_node(const char *content);
void			print_list(t_token *head);
void			free_token_list2(t_token_list *list);
void			free_token_segment(t_token *token_segment);

////////////////////////////////////////////////////////////////////

t_token			*copy_till_pipe(t_data **data);
t_token			*copy_token_segment(t_token *start, t_token *end);
void			split_tokens_new(t_data	**data);
t_token			*copy_token(t_token *token);
void			free_token_list(t_token_list *list);
void			print_token_lists(t_token_list *token_lists);
char			*retrieve_line(t_env_list *envp);
int				env_parser(t_data **data, char **envp);
char			*ft_strcat(char *dest, char *src);
void			free_token_segment(t_token *start);
t_token_list	*create_and_link(t_token *start, t_token_list *result,
					t_token_list *current_list);
t_token_list	*terminate_segment(t_token *prev);
void			free_token_segment2(t_token *start);
void			handle_parent_process(pid_t parent, int *status);
void			setup_pipe(t_data **data, int i, int prev_fd, int *end);
void			create_pipes(int *end, int pipes);
void			close_pipes(int *end, int pipes);
void			append_token(t_token **list, t_token *new_token);
t_token			*create_token(t_token_type type, char *value);
int				init_state(t_data **data, t_token **tokens, char *tmp);
int				recognizer(char *buffer, t_token **tokens,
					char *end, t_data **data);
int				token_reformatting(t_token **tokens, t_data **data);
void			token_builder(t_token **tokens, char *buffer,
					char *end, int flag);
int				find_special(char c);
void			parent_process2(t_data **data, int i, int *end);
void			process_command2(t_data **data, char **command);
void			init_extraction(t_token **result, t_token **current,
					t_data **data, t_token *tokens);
void			command_init(t_data *data, t_token *tokens);
int				exec_exit(t_data **data, t_token **tokens, int print);
int				is_whitespace(const char *str);
int				ft_isspace(int c);
void			manual_helper(t_data **data, t_token **tkn,
					char **cmd_args);
int				exec_exit2(t_data **data, t_token **tokens,
					char **cmd_args, int print);
void			command_single_helper(t_data **data);
char			*command_single_finder(int *i,
					t_data **data, char **command);
void			pipe_helper(t_data **data, t_token_list *current,
					pid_t *parent, t_token **tokens);
void			pipe_opener(t_data **data, int *end, int *flag);
void			init_pipe(t_data **data, t_token **tokens, pid_t *parent);
int				exec_exit3(t_data **data, t_token **tokens,
					int *end, int print);
int				parser_case_herdoc_pipe(t_token *current,
					t_data **data);
int				redirect_parser_pipe(t_data **data, t_token *current);
int				child_process_pipe(t_data **data, t_token *tokens,
					t_token **tkn, pid_t *parent);
void			redirection_out_case_helper(t_data **data, int *end);
void			redirection_in_case_helper(t_data **data, int *end);
void			setup_helper(t_data **data, int *end);
int				token_words(t_token *token);
void			token_reformatting_helper(t_token **current);
int				check_type(t_token *current);
int				ft_count(char const *s, char c);

#endif
