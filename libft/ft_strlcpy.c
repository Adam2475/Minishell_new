/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 20:03:41 by adapassa          #+#    #+#             */
/*   Updated: 2024/01/03 21:12:08 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	DESCRIPTION
	The strlcpy() and strlcat() functions copy and concatenate strings 
	respectively. They are designed to be safer, more consistent, and less 
	error prone replacements for strncpy(3) and strncat(3). Unlike those 
	functions, strlcpy() and strlcat() take the full size of the buffer 
	(not just the length) and guarantee to NUL-terminate the result (as 
	long as size is larger than 0 or, in the case of strlcat(), as long as 
	there is at least one byte free in dst). Note that a byte for the NULL 
	should be included in size. Also note that strlcpy() and strlcat() only 
	operate on true “C” strings.  This means that for strlcpy() src must be 
	NUL-terminated and for strlcat() both src and dst must be 
	NUL-terminated.

	The strlcpy() function copies up to size - 1 characters from the 
	NULL-terminated string src to dst, NULL-terminating the result.

	RETURN VALUE
	The strlcpy() and strlcat() functions return the total length of the 
	string they tried to create. For strlcpy() that means the length of src. 
	While this may seem somewhat confusing, it was done to make truncation 
	detection simple.
*/

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}
