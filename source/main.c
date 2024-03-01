/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 09:25:57 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/01 15:03:20 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <pthread.h>
#include <sys/time.h>

int	is_dead(t_philo *philo);

int get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf(" Error getting time ");
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

void ft_sleep(int ms, t_philo *philo)
{
	int timestop;

	timestop = get_time() + ms;
	while(get_time() < timestop)
	{
		if(is_dead(philo) == 1)
			break;
		usleep(100);
	}
}

int	is_dead(t_philo *philo)
{
	int	t;
	t = 0;
	if ((get_time() - philo->time_last_eat) > philo->rules->tt_die) // peut etre <=
	{
		pthread_mutex_lock(&philo->rules->died_lock);
		philo->rules->died = 1;
		pthread_mutex_unlock(&philo->rules->died_lock);
	}
	if (philo->rules->died == 1)
		t = 1;
	return(t);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->rules->print_lock);
	printf("[🍴] %d %d has taken a fork\n", get_time() - philo->rules->time_start, philo->id);
	pthread_mutex_unlock(&philo->rules->print_lock);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->rules->print_lock);
	printf("[🍴] %d %d has taken a fork \n", get_time() - philo->rules->time_start, philo->id);
	printf("[🍔] %d %d is eating\n", get_time() - philo->rules->time_start, philo->id);
	pthread_mutex_unlock(&philo->rules->print_lock);
	philo->time_last_eat = get_time();
	// printf(" %d %d time before\n", get_time() - philo->rules->time_start, philo->id);
	ft_sleep(philo->rules->tt_eat, philo);
	// printf(" %d %d time after\n", get_time() - philo->rules->time_start, philo->id);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->print_lock);
	printf("[💭] %d %d is thinking\n", get_time() - philo->rules->time_start, philo->id);
	pthread_mutex_unlock(&philo->rules->print_lock);
	ft_sleep(philo->rules->tt_sleep, philo);
}

void	sleeping(t_philo	*philo)
{
	pthread_mutex_lock(&philo->rules->print_lock);
	printf("[💤] %d %d is sleeping\n", get_time() - philo->rules->time_start, philo->id);
	pthread_mutex_unlock(&philo->rules->print_lock);
	ft_sleep(philo->rules->tt_sleep, philo);
}

void	*routine(void *philo_ptr)
{
	t_philo *philo;

	philo = philo_ptr;
	while(1)
	{
		eating(philo);
		if (is_dead(philo) == 1)
			break;
		sleeping(philo);
		thinking(philo);
	}
	return(NULL);
}

void life(t_rules rules)
{
	int	i;
	pthread_t *threads;

	threads = ft_calloc(rules.nbr + 1, sizeof(pthread_t));
	i = 0;
	rules.time_start = get_time();
	while(i < rules.nbr)
	{
		usleep(5);
		rules.philo[i].id = i + 1;
		rules.philo[i].rules = &rules;
		rules.philo[i].time_last_eat = get_time();
		pthread_create(&threads[i], NULL, routine, (void*)&rules.philo[i]);
		i++;
	}
	i = 0;
	while(i < rules.nbr)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
}

void set_rules(t_rules *rules, int argc, char **argv)
{
	int i;

	i = 0;
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
	rules->forks = ft_calloc(argc - 1, sizeof(pthread_mutex_t));
	pthread_mutex_init(&rules->died_lock, NULL);
	pthread_mutex_init(&rules->time_lock, NULL);
	pthread_mutex_init(&rules->print_lock, NULL);
	while(i < rules->nbr)
		pthread_mutex_init(&rules->forks[i++], NULL);
	rules->philo = ft_calloc(argc, sizeof(t_philo));

	printf("philo: %d\nTime to die: %d\nTime to eat: %d\nTime to sleep: %d\nEat: %d\n\n", rules->nbr, rules->tt_die, rules->tt_eat, rules->tt_sleep, rules->nbr_eat);
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
		i++;
	}
	temp = rules->philo[rules->nbr - 1].left_fork;
	rules->philo[rules->nbr - 1].left_fork = rules->philo[rules->nbr - 1].right_fork;
	rules->philo[rules->nbr - 1].right_fork = temp;
	rules->philo[0].left_fork = &rules->forks[i - 1];
}

void free_rules(t_rules *rules)
{
	int i;

	i = 0;
	pthread_mutex_destroy(&rules->died_lock);
	pthread_mutex_destroy(&rules->print_lock);
	pthread_mutex_destroy(&rules->time_lock);
	while(i < rules->nbr)
		pthread_mutex_destroy(&rules->forks[i++]);
	free(rules->philo);
}

int main(int argc, char **argv)
{
	t_rules rules;

	set_rules(&rules, argc, argv);
	set_philo(&rules);
	life(rules);
	free_rules(&rules);
}
