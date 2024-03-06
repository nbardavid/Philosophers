/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:48:17 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/06 15:06:52 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int ft_abs(int nbr)
{
	if (nbr > 0)
		return(nbr);
	return(-nbr);
}

void check_argc(int argc)
{
	if (argc < 5)
	{
		printf("Too few arguments\n");
		exit(EXIT_FAILURE);
	}
	if (argc > 6)
	{
		printf("Too many arguments\n");
		exit(EXIT_FAILURE);
	}
}

void check_args(t_rules *rules, int argc, char **argv)
{
	rules->nbr = ft_atoi(argv[1]);
	rules->tt_die = ft_atoi(argv[2]);
	rules->tt_eat = ft_atoi(argv[3]);
	rules->tt_sleep = ft_atoi(argv[4]);
	rules->tt_think = ft_abs(rules->tt_eat - rules->tt_sleep);
	rules->died = 0;
	if (argc == 6)
		rules->nbr_eat = ft_atoi(argv[5]);
	else
		rules->nbr_eat = INT_MAX;
	if (rules->nbr < 0 || rules->tt_sleep < 0 || rules->tt_eat < 0 || rules->nbr_eat < 0)
	{
		printf("Argument(s) are negatives values\n");
		exit(EXIT_FAILURE);
	}
	if (rules->nbr > 200)
	{
		printf("Too many philosophers\n");
		exit(EXIT_FAILURE);
	}
}

void set_rules(t_rules *rules, int argc, char **argv)
{
	int i;

	i = 0;
	check_argc(argc);
	check_args(rules, argc, argv);
	rules->forks = ft_calloc(rules->nbr + 1, sizeof(pthread_mutex_t));
	if (rules->forks == NULL)
		exit (EXIT_FAILURE);
	rules->philo = ft_calloc(rules->nbr + 1, sizeof(t_philo));
	if (rules->philo == NULL)
	{
		free(rules->forks);
		exit (EXIT_FAILURE);
	}
	if (pthread_mutex_init(&rules->died_lock, NULL) != 0 ||
		pthread_mutex_init(&rules->time_lock, NULL) != 0 || 
		pthread_mutex_init(&rules->print_lock, NULL) != 0)
	{
		printf("Error initializing mutexs\n");
		exit (EXIT_FAILURE);
	}
	while(i < rules->nbr)
		if (pthread_mutex_init(&rules->forks[i++], NULL) != 0)
			exit (EXIT_FAILURE);
}

void set_philo(t_rules *rules)
{
	int	i;
	pthread_mutex_t *temp;

	i = 0;
	while(i < rules->nbr)
	{
		if (i != 0)
			rules->philo[i].left_fork = &rules->forks[i - 1];
		rules->philo[i].right_fork = &rules->forks[i];
		rules->philo[i].nbr_eat = 0;
		if (pthread_mutex_init(&rules->philo[i].eat_lock, NULL) != 0)
		{
			printf("Error initializing eat_lock mutexs\n");
			exit (EXIT_FAILURE);
		}
		i++;
	}
	if (rules->nbr > 1)
	{
		temp = rules->philo[rules->nbr - 1].left_fork;
		rules->philo[rules->nbr - 1].left_fork = rules->philo[rules->nbr - 1].right_fork;
		rules->philo[rules->nbr - 1].right_fork = temp;
		rules->philo[0].left_fork = &rules->forks[i - 1];
	}
}
