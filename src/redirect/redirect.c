/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/24 19:20:25 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Helper function to check if the content of a node is all whitespace
int is_whitespace(const char *str)
{
	while (*str)
	{
		if (!isspace((unsigned char)*str))
			return 0;
		str++;
	}
	return 1;
}

// Function to remove whitespace nodes
void remove_whitespace_nodes(t_token **head)
{
	t_token	*current = *head;
	t_token	*prev = NULL;

	while (current != NULL)
	{
		if (is_whitespace(current->value))
		{
			if (prev == NULL)
			{
				*head = current->next;
				free(current->value);
				free(current);
				current = *head;
			}
			else
			{
				prev->next = current->next;
				free(current->value);
				free(current);
				current = prev->next;
			}
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

// Function to create a new list node
t_token *new_node(const char *content)
{
	t_token *node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return NULL;
	node->value = ft_strdup(content);
	node->type = 0;
	node->next = NULL;
	return node;
}

char	*trim_whitespace(char *str)
{
	char *end;

	while (*str == 32) str++;
	if (*str == 0)
		return str;
	end = str + strlen(str) - 1;
	while (end > str && *end == 32) end--;
	*(end + 1) = '\0';
	return (str);
}

int	execute_command(char *command, t_data **data, char **envp)
{
	char *cmd;
	char **cmd_args;
	char *tmp;
	char *holder;

	cmd_args = ft_split(command, 32);
	cmd = cmd_args[0];
	tmp = NULL;
	holder = find_cmd(cmd, data);

	int i = 1;
	while (cmd_args[i])
	{
		tmp = ft_strjoin_gnl(tmp, trim_whitespace(cmd_args[i]));
		i++;
	}
	execve(holder, cmd_args, envp);
	return(0);
}

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
	return (i);
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

char	*exp_word(char *line, t_data **data, int (*i))
{
	char		*tmp;
	char		*res;
	int			count;

	count = (*i);
	while (line[(*i)] && (ft_isalnum(line[(*i)]) || line[(*i)] == '_'))
		(*i)++;
	count = (*i) - count;
	tmp = (char *)ft_calloc(sizeof(char), count + 1);
	(*i) -= count;
	count = 0;
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
	int		len_var;
	int		j;
	char	*pre_doll;
	char	*post_doll;

	len_var = ft_strlen(env);
	if (*line != '$')
		pre_doll = ft_strndup(line, (ft_strlen_char(line, '$') - 1));
	else
		pre_doll = ft_strndup("", 0);
	j = -1;
	while (line[i] != '\0' && line[i] != 32
	&& line[i] != '\'' && line[i] != '\"')
		i++;
	post_doll = ft_calloc(sizeof(char), (ft_strlen(line) - i) + 1);
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

static	char	*expander_doc(char *line, t_data **data)
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

void	handle_heredoc(char *delimiter, t_data **data, char *tmp)
{
	char		*line;
	int			heredoc_fd;

	if ((*data)->fd < 0)
		exit (ft_printf("Failed to open heredoc temporary file"));
	signal_doc();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		if (ft_strsearch(line, '$'))
			line = expander_doc(line, data);
		write((*data)->fd, line, ft_strlen(line));
		write((*data)->fd, "\n", 1);
		free(line);
	}
	close((*data)->fd);
	heredoc_fd = open(tmp, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror("Failed to reopen heredoc temporary file");
		exit(0);
	}
	while (1)
	{
		line = get_next_line2(heredoc_fd);
		if (!line)
			break ;
		// da togliere
		// ft_printf("%s", line);
		free(line);
	}
	close (heredoc_fd);
	exit(0);
}
