/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:19:19 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/07 15:47:12 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h" 
#include <fcntl.h>

void	ft_sleep(int ms)
{
	int	timestop;

	timestop = get_time() + ms;
	while (get_time() < timestop)
	{
		usleep(100);
	}
}

int	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf(" Error getting time ");
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void init_sem(t_rules *rules)
{
	rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nbr);
	rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
}


void routine(t_philo *philo)
{
	int i;

	i = 0;
	fflush(stdout);
	while(i < 10)
	{
		printf("hello\n");
		eat(philo);
		i++;
	}
}

void life(t_rules *rules)
{
	int	i;
	int id;

	i = 0;
	rules->time_start = get_time();
	while(i < rules->nbr)
	{
		id = fork();
		if (id == 0)
		{
			rules->philo[i].id = i;
			routine(&rules->philo[i]);
			return ;
		}
		i++;
		usleep(10);
	}
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	set_rules(&rules, argc, argv);
	init_sem(&rules);
	life(&rules);
	return (0);
}

// ./philo dsdas sd adas dasdasdasda
