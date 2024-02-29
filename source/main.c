/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 09:25:57 by nbardavi          #+#    #+#             */
/*   Updated: 2024/02/29 15:43:07 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*routine(void *philo_ptr)
{
	t_philo *philo;

	philo = philo_ptr;
	printf("HELLO");
	printf("Philo %d\n", philo->id + 1);
	pthread_join();//remplir
	return(NULL);
}

void life(t_rules rules)
{
	int	i;
	pthread_t *threads;

	threads = ft_calloc(rules.nbr + 1, sizeof(pthread_t));
	i = 0;
	printf("%d\n", rules.nbr);
	while(i < rules.nbr)
	{
		usleep(1000);
		rules.philo[i].id = i;
		rules.philo[i].rules = rules;
		pthread_create(&threads[i], NULL, routine, (void*)&rules.philo[i]);
		i++;
	}
}

void set_rules(t_rules *rules, int argc, char **argv)
{
	if (argc < 6)
	{
		printf("Too few argmuents\n");
		exit(EXIT_FAILURE);
	}
	else if (argc > 6)
	{
		printf("Too many arguments\n");
		exit(EXIT_FAILURE);
	}
	rules->nbr = ft_atoi(argv[1]);
	rules->tt_die = ft_atoi(argv[2]);
	rules->tt_eat = ft_atoi(argv[3]);
	rules->tt_sleep = ft_atoi(argv[4]);
	rules->nbr_eat = ft_atoi(argv[5]);
	rules->forks = ft_calloc(argc - 1, sizeof(int));
	rules->philo = ft_calloc(argc, sizeof(t_philo));
}

int main(int argc, char **argv)
{
	t_rules rules;

	set_rules(&rules, argc, argv);
	life(rules);
}
