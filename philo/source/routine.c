/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:11:33 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/06 12:25:19 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->print_lock);
	printf("[💭] %d %d is thinking\n", get_time() - philo->rules->time_start,
		philo->id);
	pthread_mutex_unlock(&philo->rules->print_lock);
	ft_sleep(philo->rules->tt_think, philo);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->print_lock);
	printf("[💤] %d %d is sleeping\n", get_time() - philo->rules->time_start,
		philo->id);
	pthread_mutex_unlock(&philo->rules->print_lock);
	ft_sleep(philo->rules->tt_sleep, philo);
}

int	check_dead(t_philo *philo, int *trigger)
{
	pthread_mutex_lock(&philo->rules->died_lock);
	if (philo->rules->died == 1)
	{
		*trigger = 1;
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->died_lock);
	return (0);
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;
	int		trigger;

	trigger = 0;
	philo = philo_ptr;
	while (1)
	{
		eating(philo);
		if (check_dead(philo, &trigger) == 1)
			break ;
		sleeping(philo);
		if (check_dead(philo, &trigger) == 1)
			break ;
		if (philo->rules->tt_think > 0)
			thinking(philo);
		if (check_dead(philo, &trigger) == 1)
			break ;
	}
	if (trigger == 1)
		pthread_mutex_unlock(&philo->rules->died_lock);
	return (NULL);
}
