/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:13:04 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/06 13:48:44 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_atoi(char *str)
{
	unsigned long	nbr;
	int				i;
	int				sign;

	sign = 1;
	i = 0;
	nbr = 0;
	if (!str)
		return (0);
	if (str[0] == '-')
	{
		i++;
		sign = -1;
	}
	else if (str[0] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		nbr *= 10;
		nbr += str[i++] - '0';
	}
	if (str[i] != '\0')
		return (0);
	return (nbr * sign);
}
