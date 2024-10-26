/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:29:52 by adapassa          #+#    #+#             */
/*   Updated: 2024/01/04 20:33:55 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
    The calloc() function allocates memory for an array of  nmemb  elements
    of  size bytes each and returns a pointer to the allocated memory.  The
    memory is set to zero.  If nmemb or size is 0,  then  calloc()  returns
    either  NULL,  or a unique pointer value that can later be successfully
    passed to free().  If the multiplication of nmemb and size would result
    in  integer  overflow, then calloc() returns an error.
*/

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_memset ((unsigned char *)ptr, 0, nmemb * size);
	return (ptr);
}
