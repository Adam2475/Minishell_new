/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 20:55:25 by adapassa          #+#    #+#             */
/*   Updated: 2024/01/08 15:19:46 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = ft_strlen(dest);
	if (size <= j)
		return (size + ft_strlen(src));
	while ((src[i] != '\0') && ((j + 1) < size))
		dest[j++] = src[i++];
	dest[j] = '\0';
	return (ft_strlen(dest) + ft_strlen(&src[i]));
}
