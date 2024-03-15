/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:27:40 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/11 09:21:39 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	is_dead(t_philo *philo)
{
	int	t;

	t = 0;
	pthread_mutex_lock(&philo->eat_lock);
	if ((get_time() - philo->time_last_eat) > philo->rules->tt_die)
	{
		pthread_mutex_lock(&philo->rules->print_lock);
		pthread_mutex_lock(&philo->rules->died_lock);
		philo->rules->died = 1;
		printf("[💀] %d %d died\n", get_time() - philo->rules->time_start,
			philo->id);
		pthread_mutex_unlock(&philo->rules->died_lock);
		pthread_mutex_unlock(&philo->rules->print_lock);
	}
	pthread_mutex_unlock(&philo->eat_lock);
	if (philo->rules->died == 1)
		t = 1;
	return (t);
}

int	eat_all_print(t_rules *rules)
{
	pthread_mutex_lock(&rules->died_lock);
	rules->died = 1;
	pthread_mutex_unlock(&rules->died_lock);
	pthread_mutex_lock(&rules->print_lock);
	printf("[🍔] %d All the philosophers are satisfied\n", get_time()
		- rules->time_start);
	pthread_mutex_unlock(&rules->print_lock);
	return (0);
}

void	eat_all_checker(t_rules *rules, int i, int *trigger)
{
	pthread_mutex_lock(&rules->philo[i].eat_lock);
	if (rules->philo[i].nbr_eat < rules->nbr_eat)
		*trigger = 1;
	pthread_mutex_unlock(&rules->philo[i].eat_lock);
}

void	*checker(void *rules_ptr)
{
	t_rules	*rules;
	int		i;
	int		trigger;

	i = 0;
	rules = rules_ptr;
	while (1)
	{
		trigger = 0;
		while (i < rules->nbr)
		{
			is_dead(&rules->philo[i]);
			eat_all_checker(rules, i, &trigger);
			if (rules->died == 1)
				break ;
			i++;
		}
		if (rules->died == 1)
			break ;
		if (trigger == 0 && eat_all_print(rules) == 0)
			break ;
		usleep(100);
		i = 0;
	}
	return (NULL);
}
