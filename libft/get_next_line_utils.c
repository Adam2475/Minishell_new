/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:19:19 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/13 18:56:36 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_free_mat(char **mat, char *str)
{
	if (!mat && !str)
		return (NULL);
	else if ((mat != NULL) && (str != NULL))
	{
		free(mat[1]);
		free(mat[0]);
		free(mat);
		free(str);
	}
	else if (str == NULL)
	{
		free(mat[1]);
		free(mat[0]);
		free(mat);
	}
	else if (mat == NULL)
		free(str);
	return (NULL);
}

void	*ft_custom_function(size_t nmemb, size_t size, char *str, bool flag)
{
	char		*ptr;
	size_t		i;
	int			length;

	i = 0;
	length = 0;
	if (flag == TRUE)
	{
		ptr = (char *)malloc(nmemb * size);
		if (!ptr)
			return (NULL);
		while (i < (nmemb * size))
			ptr[i++] = 0;
		return ((void *)ptr);
	}
	else
	{
		while (str && str[length] != '\0')
			length++;
		return ((void *)(long int)length);
	}
}

char	*ft_strdup_gnl(const char *src)
{
	char		*dest;
	long int	l;
	size_t		i;

	i = -1;
	if (!src)
		return (NULL);
	l = (long int)ft_custom_function(0, 0, (char *)src, 0);
	dest = malloc(sizeof(char) * (l + 1));
	if (!src)
		return (0);
	if (!dest)
		return (0);
	while (++i <= (size_t)l)
		((char *)dest)[i] = ((char *)src)[i];
	dest[l] = '\0';
	return (dest);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*s3;
	int		i;
	int		j;

	i = 0;
	j = -1;
	s3 = (char *)ft_custom_function(sizeof(char),
			((long)ft_custom_function(0, 0, (char *)s1, 0)
				+ (long)ft_custom_function(0, 0, (char *)s2, 0) + 1), NULL, 1);
	if (!s3)
		return (0);
	if (s1)
	{
		while (s1[i] != '\0')
		{
			s3[i] = s1[i];
			i++;
		}
	}
	while (s2[++j] != '\0')
		s3[i + j] = s2[j];
	s3[i + j] = '\0';
	free(s1);
	return (s3);
}

char	**ft_minisplit(char *str)
{
	char	**tmp;
	int		i;
	int		j;

	i = 0;
	tmp = (char **)ft_custom_function(sizeof(char *), 2, NULL, 1);
	while (str[i] != '\n')
		i++;
	tmp[0] = (char *)ft_custom_function(sizeof(char), (i + 1), NULL, 1);
	i = 0;
	while (str[i] != '\0')
		i++;
	tmp[1] = ft_custom_function(sizeof(char), (i + 1), NULL, 1);
	i = -1;
	while (str[++i] != '\n')
		tmp[0][i] = str[i];
	tmp[0][i] = '\0';
	j = i + 1;
	i = 0;
	while (str[j] != '\0')
		tmp[1][i++] = str[j++];
	tmp[1][i] = '\0';
	return (tmp);
}
