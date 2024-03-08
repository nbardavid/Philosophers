/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 09:25:57 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/08 21:00:48 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	exit_thread(t_rules rules)
{
	write(2, "Error creating threads\n", 23);
	free_rules(&rules);
	exit(EXIT_FAILURE);
}

void	solo_life(t_rules rules)
{
	rules.time_start = get_time();
	printf("[🍴] %d 1 has taken a fork \n", get_time() - rules.time_start);
	usleep(rules.tt_die * 1000);
	printf("[💀] %d 1 died\n", get_time() - rules.time_start);
}

void	life(t_rules rules)
{
	int			i;
	pthread_t	*threads;

	threads = ft_calloc(rules.nbr + 1, sizeof(pthread_t));
	if (threads == NULL)
	{
		write(2, "Error allocating memory\n", 23);
		return;
	}
	i = 0;
	rules.time_start = get_time();
	while (i < rules.nbr)
	{
		usleep(10);
		rules.philo[i].id = i;
		rules.philo[i].rules = &rules;
		rules.philo[i].time_last_eat = get_time();
		if (pthread_create(&threads[i], NULL, routine,
				(void *)&rules.philo[i]) != 0)
			exit_thread(rules);
		i++;
	}
	if (pthread_create(&threads[i], NULL, checker, (void *)&rules))
		exit_thread(rules);
	i = 0;
	while (i < rules.nbr + 1)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	set_rules(&rules, argc, argv);
	set_philo(&rules);
	if (rules.nbr > 1)
		life(rules);
	else
		solo_life(rules);
	free_rules(&rules);
}
