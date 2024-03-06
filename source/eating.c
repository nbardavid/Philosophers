/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:46 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/06 14:39:41 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int		check_dead_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->died_lock);
	if (philo->rules->died == 1)
		return (1);
	pthread_mutex_unlock(&philo->rules->died_lock);
	return (0);
}

int get_left_fork(t_philo *philo)
{
	if (check_dead_fork(philo) == 1)
	{
		pthread_mutex_unlock(&philo->rules->died_lock);
		return (1);
	}
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->rules->print_lock);
	if (check_dead_fork(philo) == 0)
		printf("[🍴] %d %d has taken a fork\n", get_time() - philo->rules->time_start, philo->id);
	else
		pthread_mutex_unlock(&philo->rules->died_lock);
	pthread_mutex_unlock(&philo->rules->print_lock);
	return (0);
}

int get_right_fork(t_philo *philo)
{
	if (check_dead_fork(philo) == 1)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(&philo->rules->died_lock);
		return (1);
	}
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->rules->print_lock);
	if (check_dead_fork(philo) == 0)
	{
		printf("[🍴] %d %d has taken a fork \n", get_time() - philo->rules->time_start, philo->id);
		printf("[🍔] %d %d is eating\n", get_time() - philo->rules->time_start, philo->id);
	}
	else
		pthread_mutex_unlock(&philo->rules->died_lock);
	return (0);
}

void eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_lock);
	philo->nbr_eat++;
	pthread_mutex_unlock(&philo->eat_lock);
	pthread_mutex_lock(&philo->eat_lock);
	philo->time_last_eat = get_time();
	pthread_mutex_unlock(&philo->eat_lock);
	ft_sleep(philo->rules->tt_eat, philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	eating(t_philo *philo)
{
	if (get_left_fork(philo) == 1)
		return ;
	if (get_right_fork(philo) == 1)
		return ;

	pthread_mutex_unlock(&philo->rules->print_lock);
	eat(philo);
}
