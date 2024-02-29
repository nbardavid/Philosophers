/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:42:09 by nbardavi          #+#    #+#             */
/*   Updated: 2024/02/29 14:42:29 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*returnvalue;

	if (size == 0)
	{
		returnvalue = malloc(0);
		return (returnvalue);
	}
	if (nmemb <= 0 && size <= 0)
		return (NULL);
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	returnvalue = malloc(nmemb * size);
	if (returnvalue == NULL || (!nmemb && !size))
		return (NULL);
	if (returnvalue)
		memset(returnvalue, 0, nmemb * size);
	return (returnvalue);
}
