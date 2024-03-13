/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:19:19 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/13 16:16:42 by nbardavi         ###   ########.fr       */
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
	(void)rules;
	// rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nbr);
	// rules->dead_lock = sem_open("/lock", O_CREAT, 0644, 1);
	// rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
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
		sem_wait(philo->rules->print_lock);
		sem_post(philo->rules->print_lock);
	}
	else if (t == 2)
	{
		sem_close(philo->rules->forks);
		sem_close(philo->rules->dead_lock);
		sem_close(philo->rules->print_lock);
		exit (EXIT_SUCCESS);
	}
	sem_close(philo->rules->forks);
	sem_close(philo->rules->dead_lock);
	sem_close(philo->rules->print_lock);
	exit(EXIT_DEAD);
}

void	sleeping(t_philo *philo)
{
	sem_wait(philo->rules->print_lock);
	printf("[💤] %d %d is sleeping\n", get_time() - philo->rules->time_start, philo->id);
	sem_post(philo->rules->print_lock);
	ft_sleep(philo->rules->tt_sleep, philo);
}

void	thinking(t_philo *philo)
{
	sem_wait(philo->rules->print_lock);
	printf("[💭] %d %d is thinking\n", get_time() - philo->rules->time_start, philo->id);
	sem_post(philo->rules->print_lock);
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

void kill_them_all(t_rules *rules)
{
	int	i;

	i = 0;
	while(i < rules->nbr)
	{
		kill(rules->id[i], SIGKILL);
		i++;
	}
}

void *bigbrother(void *rules_ptr)
{
	t_rules	*rules;
	int id;

	rules = (t_rules *)rules_ptr;
	waitpid(-1, &id, 0);
	id = WEXITSTATUS(id);
	if (id == EXIT_DEAD)
	{
		rules->trigger = 1;
		kill_them_all(rules);
	}
	else
		rules->trigger = 0;
	return (NULL);
}

void life(t_rules *rules)
{
	int	i;
	pthread_t	threads;

	rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nbr);
	rules->dead_lock = sem_open("/dead_lock", O_CREAT, 0644, 1);
	rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
	i = 0;
	while(i < rules->nbr)
	{
		rules->time_start = get_time();
		rules->philo[i].id = i;
		rules->philo[i].time_last_eat = get_time();
		rules->philo[i].nbr_eat = 0;
		rules->id[i] = fork();
		if (rules->id[i] == 0)
		{
			routine(&rules->philo[i]);
			exit(EXIT_SUCCESS);
		}
		i++;
		usleep(250 * rules->nbr);
	}
	if (pthread_create(&threads, NULL, bigbrother, rules) == -1)
	{
		printf("thread failed\n");
		exit (EXIT_FAILURE);
		// exit_thread(rules);
	}
	i = 0;
	pthread_join(threads, NULL);
}

void clean(t_rules *rules)
{
	sem_close(rules->forks);
	sem_unlink("/forks");
	sem_close(rules->dead_lock);
	sem_unlink("/dead_lock");
	sem_close(rules->print_lock);
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
