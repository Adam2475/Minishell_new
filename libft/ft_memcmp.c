/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 17:45:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/01/04 20:35:57 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	RETURN VALUE
    The memcmp() function returns  an  integer  less  than,  equal  to,  or
    greater than zero if the first n bytes of s1 is found, respectively, to
    be less than, to match, or be greater than the first n bytes of s2.

    For a nonzero return value, the sign is determined by the sign  of  the
    difference  between  the  first  pair of bytes (interpreted as unsigned
    char) that differ in s1 and s2.

    If n is zero, the return value is zero.
*/

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((size_t)i < n)
	{
		if (((char *)s1)[i] != ((char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		else
			i++;
	}
	return (0);
}
