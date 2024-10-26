/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 10:21:22 by adapassa          #+#    #+#             */
/*   Updated: 2024/02/08 17:31:52 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printhex(unsigned long nbr, int maiusc)
{
	char	*container;
	char	*maiuscarr;
	int		len;
	int		tmp;

	container = "0123456789abcdef";
	maiuscarr = "0123456789ABCDEF";
	len = 0;
	if (nbr == 0)
		return (write(1, "0", 1));
	if (nbr > 0)
	{
		tmp = nbr % 16;
		if ((nbr / 16) > 0)
			len += ft_printhex((nbr /= 16), maiusc);
		if (maiusc == FALSE)
			len += ft_putchar(container[tmp]);
		else if (maiusc == TRUE)
			len += ft_putchar(maiuscarr[tmp]);
	}
	return (len);
}

int	ft_printptr(unsigned long address)
{
	unsigned long long	tmp;
	int					len;

	len = 0;
	if (!address)
		return (write(1, "(nil)", 5));
	tmp = (unsigned long long)address;
	len += write(1, "0x", 2);
	len += ft_printhex(tmp, 0);
	return (len);
}

int	ft_print_unsinged(unsigned long n)
{
	int	len;

	len = 0;
	if (n > 9)
	{
		len += ft_print_unsinged(n / 10);
		len += ft_print_unsinged(n % 10);
	}
	else
		len += ft_putchar(n + 48);
	return (len);
}
