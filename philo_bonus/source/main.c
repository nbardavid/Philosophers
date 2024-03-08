/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:19:19 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/08 15:51:59 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h" 

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

int	check_dead(t_philo *philo)
{
	if (philo->rules->tt_eat < (get_time() - philo->time_last_eat))
		return(1);
	return(0);
}

void suicide(t_philo *philo)
{
	(void)philo;
	printf("pan\n");
	//exit propre;
	exit(EXIT_DEAD);
}

void routine(t_philo *philo)
{
	int i;

	i = 0;
	while(1)
	{
		eat(philo);
		if (check_dead(philo) == 1)
			suicide(philo);
		// sleep(philo);
		// thinking(philo);
		i++;
	}
}

void kill_them_all(t_philo *philo, int skip_id)
{
	(void)philo;
	int	i;

	i = 0;
	while(i < philo->rules->nbr)
	{
		if (philo->rules->id[i] != skip_id)
			kill(philo->rules->id[i], SIGKILL);
		printf("pan pan\n");
		i++;
	}
}

void *bigbrother(void *philo_ptr)
{
	t_philo	*philo;

	philo = philo_ptr;
	if (waitpid(philo->rules->id[philo->id], NULL, 0) == EXIT_DEAD)
	{
		printf("[💀] %d %d died\n", get_time() - philo->rules->time_start, philo->id);
		kill_them_all(philo, philo->rules->id[philo->id]);
	}
	return (NULL);
}

void life(t_rules *rules)
{
	int	i;
	pthread_t	*threads;

	threads = ft_calloc(rules->nbr, sizeof(pthread_t));
	i = 0;
	rules->time_start = get_time();
	while(i < rules->nbr)
	{
		rules->philo[i].id = i;
		if (pthread_create(&threads[i], NULL, bigbrother, (void *)&rules->philo[i]) != 0)
		{
			printf("thread failed\n");
			exit (EXIT_FAILURE);
			// exit_thread(rules);
		}
		rules->id[i] = fork();
		if (rules->id[i] == 0)
		{
			routine(&rules->philo[i]);
			exit(EXIT_SUCCESS);
		}
		i++;
		usleep(10);
	}
	i = 0;
	while(i < rules->nbr)
		pthread_join(threads[i++], NULL);
}

void clean(t_rules *rules)
{
	sem_close(rules->forks);
	sem_unlink("/forks");
	rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
	sem_unlink("/print");
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	set_rules(&rules, argc, argv);
	init_sem(&rules);
	life(&rules);
	clean(&rules);
	return (0);
}

// ./philo dsdas sd adas dasdasdasda
// faire un thread quand un seul philo
