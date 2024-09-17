/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:24:07 by adapassa          #+#    #+#             */
/*   Updated: 2024/01/08 15:23:34 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(char const *s, char c)
{
	int	i;
	int	sn;

	i = 0;
	sn = 0;
	while (s[i] != 0)
	{
		if (s[i] != c)
		{
			sn++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		if (s[i] != '\0')
			i++;
	}
	return (sn);
}

static size_t	ft_superstrlen(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	**malloc_error(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

char	**main_procedure(char **str, int *i, char *s, char c)
{
	while (s[++i[0]] != 0)
	{
		if (s[i[0]] != c)
		{
			str[i[1]] = ft_substr(s, i[0], ft_superstrlen(&s[i[0]], c));
			if (!str[i[1]])
				return (malloc_error(str));
			i[0] = i[0] + ft_superstrlen(&s[i[0]], c);
			break ;
		}
	}
	i[1]++;
	return (str);
}

char	**ft_split(const char *s, char c)
{
	int		ns;
	char	**str;
	int		i[2];

	i[1] = 0;
	i[0] = -1;
	ns = ft_count(s, c);
	str = ft_calloc((ns + 1), sizeof(char *));
	if (!str)
		return (NULL);
	while (ns--)
	{
		if (main_procedure(str, i, (char *)s, c) == NULL)
			return (NULL);
	}
	str[i[1]] = 0;
	return (str);
}
