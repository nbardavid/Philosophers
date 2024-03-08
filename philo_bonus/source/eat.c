/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 12:13:31 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/08 15:49:51 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// void print_message(t_philo *philo, char* emoji, char *msg)
// {
// 	fd_putstr(1, "[");
// 	fd_putstr(1, emoji);
// 	fd_putstr(1, "] ");
// 	fd_putnbr(1, get_time());
// 	write(1, " ", 1);
// 	fd_putnbr(1, philo->id);
// 	write(1, "\n", 1);
// 	fd_putstr(1, msg);
// }

void get_left_fork(t_philo *philo)
{
	sem_wait(philo->rules->forks);
	printf("[🍴] %d %d has taken a fork\n", get_time() - philo->rules->time_start, philo->id);
}

void get_right_fork(t_philo *philo)
{
	sem_wait(philo->rules->forks);
	printf("[🍴] %d %d has taken a fork\n", get_time() - philo->rules->time_start, philo->id);
}

void eating (t_philo *philo)
{
	printf("[🍔] %d %d is eating\n", get_time() - philo->rules->time_start,
		philo->id);
	ft_sleep(philo->rules->tt_eat);
	sem_post(philo->rules->forks);
	sem_post(philo->rules->forks);
}

void eat(t_philo *philo)
{
	get_left_fork(philo);
	get_right_fork(philo);
	eating(philo);
}

