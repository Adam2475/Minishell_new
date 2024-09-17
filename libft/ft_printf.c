/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 10:21:25 by adapassa          #+#    #+#             */
/*   Updated: 2024/02/08 17:47:52 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

static int	ft_putnbr(int n)
{
	int	len;

	len = 0;
	if (n == -2147483648)
		len += write(1, "-2147483648", 11);
	else if (n < 0)
	{
		len += write(1, "-", 1);
		n = -n;
		len += ft_putnbr(n);
	}
	else if (n > 9)
	{
		len += ft_putnbr(n / 10);
		len += ft_putnbr(n % 10);
	}
	else
		len += ft_putchar(n + 48);
	return (len);
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (write(1, "(null)", 6));
	while (str[i] != '\0')
		write(1, &str[i++], 1);
	return (i);
}

int	format_selector(char c, va_list args)
{
	if (c == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(args, char *)));
	else if (c == 'x')
		return (ft_printhex((long)va_arg(args, unsigned int), 0));
	else if (c == 'X')
		return (ft_printhex((long)va_arg(args, unsigned int), 1));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr((long)va_arg(args, int)));
	else if (c == 'p')
		return ((long)ft_printptr(va_arg(args, unsigned long)));
	else if (c == '%')
		return (write(1, "%%", 1));
	else if (c == 'u')
		return (ft_print_unsinged(va_arg(args, unsigned int)));
	else
		return (write(1, &c, 1));
}

int	ft_printf(const char *format, ...)
{
	va_list		args;
	int			len;
	int			i;

	va_start(args, format);
	i = 0;
	len = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
			len += format_selector(format[++i], args);
		else
			len += write(1, &format[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}
