/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:20:49 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/07 15:30:47 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	fd_putstr(int fd, char *str)
{
	int	i;

	i = 0;
	while(str[i])
		write(fd, &str[i++], 1);
}

void	fd_putchar(int fd, char c)
{
	write(fd, &c, 1);
}

void	fd_putnbr(int n, int fd)
{
	unsigned int	nbr;

	if (n < 0)
	{
		fd_putchar('-', fd);
		nbr = -n;
	}
	else
		nbr = n;
	if (nbr > 9)
	{
		fd_putnbr(nbr / 10, fd);
		fd_putnbr(nbr % 10, fd);
	}
	else
		fd_putchar(nbr + '0', fd);
}
