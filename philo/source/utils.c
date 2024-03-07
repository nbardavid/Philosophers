/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:09:35 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/06 12:11:01 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf(" Error getting time ");
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_sleep(int ms, t_philo *philo)
{
	int	timestop;
	int	trigger;

	trigger = 0;
	(void)philo;
	timestop = get_time() + ms;
	while (get_time() < timestop)
	{
		usleep(100);
		pthread_mutex_lock(&philo->rules->died_lock);
		if (philo->rules->died == 1)
		{
			trigger = 1;
			break ;
		}
		pthread_mutex_unlock(&philo->rules->died_lock);
	}
	if (trigger == 1)
		pthread_mutex_unlock(&philo->rules->died_lock);
}

void	free_rules(t_rules *rules)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&rules->died_lock);
	pthread_mutex_destroy(&rules->print_lock);
	pthread_mutex_destroy(&rules->time_lock);
	while (i < rules->nbr)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		pthread_mutex_destroy(&rules->philo[i].eat_lock);
		i++;
	}
	free(rules->philo);
	free(rules->forks);
}
