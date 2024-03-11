/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:19:19 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/11 05:27:55 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h" 

void suicide(t_philo *philo, int t);
int	check_dead(t_philo *philo);

void	ft_sleep(int ms, t_philo *philo)
{
	int	timestop;

	(void)philo;
	timestop = get_time() + ms;
	while (get_time() < timestop)
	{
		usleep(100);
		suicide(philo, check_dead(philo));
	}
}

int	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf(" Error getting time \n");
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void init_sem(t_rules *rules)
{
	sem_unlink("/forks");
	sem_unlink("/lock");
	sem_unlink("/print");
	rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nbr);
	rules->dead_lock = sem_open("/lock", O_CREAT, 0644, 1);
	rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
}

int	check_dead(t_philo *philo)
{
	if (philo->rules->tt_die < (get_time() - philo->time_last_eat))
		return(1);
	else if (philo->nbr_eat == philo->rules->nbr_eat)
		return(2);
	return(0);
}

void suicide(t_philo *philo, int t)
{
	(void)philo;
	if (t == 0)
		return;
	if (t == 1)
	{
		sem_wait(philo->rules->dead_lock);
		printf("[💀] %d %d died\n", get_time() - philo->rules->time_start, philo->id);
	}
	else if (t == 2)
		exit (EXIT_SUCCESS);
	exit(EXIT_DEAD);
}

void	sleeping(t_philo *philo)
{
	printf("[💤] %d %d is sleeping\n", get_time() - philo->rules->time_start, philo->id);
	ft_sleep(philo->rules->tt_sleep, philo);
}

void	thinking(t_philo *philo)
{
	printf("[💭] %d %d is thinking\n", get_time() - philo->rules->time_start, philo->id);
	ft_sleep(philo->rules->tt_think, philo);
}

void routine(t_philo *philo)
{
	int i;

	i = 0;
	while(1)
	{
		eat(philo);
		suicide(philo, check_dead(philo));
		sleeping(philo);
		suicide(philo, check_dead(philo));
		thinking(philo);
		suicide(philo, check_dead(philo));
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
		i++;
	}
}

void *bigbrother(void *philo_ptr)
{
	t_philo	*philo;
	int id;

	philo = philo_ptr;
	waitpid(philo->rules->id[philo->id], &id, 0);
	id = WEXITSTATUS(id);
	if (id == EXIT_DEAD)
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
		rules->philo[i].time_last_eat = get_time();
		rules->id[i] = fork();
		if (rules->id[i] == 0)
		{
			routine(&rules->philo[i]);
			exit(EXIT_SUCCESS);
		}
		if (pthread_create(&threads[i], NULL, bigbrother, (void *)&rules->philo[i]) != 0)
		{
			printf("thread failed\n");
			exit (EXIT_FAILURE);
			// exit_thread(rules);
		}
		i++;
		usleep(10);
	}
	i = 0;
	while(i < rules->nbr)
		pthread_join(threads[i++], NULL);
	printf("[🍔] All the philosophers are satisfied\n");
	free(threads);
}

void clean(t_rules *rules)
{
	printf("fdp\n");
	sem_close(rules->forks);
	sem_close(rules->dead_lock);
	sem_close(rules->print_lock);
	sem_unlink("/forks");
	sem_unlink("/lock");
	sem_unlink("/print");
	free(rules->philo);
	free(rules->id);
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
