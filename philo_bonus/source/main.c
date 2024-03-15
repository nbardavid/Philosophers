/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:19:19 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/14 16:32:35 by nbardavi         ###   ########.fr       */
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
	sem_unlink("/fork_lock");
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
		sem_close(philo->rules->fork_lock);
		exit (EXIT_SUCCESS);
	}
	sem_close(philo->rules->forks);
	sem_close(philo->rules->dead_lock);
	sem_close(philo->rules->print_lock);
	sem_close(philo->rules->fork_lock);
	printf("[💀] %d 1 died\n", get_time() - philo->rules->time_start);
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
		sem_wait(rules->fork_lock);
		kill(rules->id[i], SIGKILL);
		sem_post(rules->fork_lock);
		i++;
	}
}

void *bigbrother(void *philo_ptr)
{
	t_philo	*philo;
	int id;
	int temp;

	philo = (t_philo *)philo_ptr;
	sem_wait(philo->rules->fork_lock);
	temp = philo->rules->id[philo->id];
	sem_post(philo->rules->fork_lock);
	waitpid(temp, &id, 0);
	id = WEXITSTATUS(id);
	if (id == EXIT_DEAD)
		kill_them_all(philo->rules);
	return (NULL);
}

void life(t_rules *rules)
{
	int	i;
	pthread_t	*threads;

	rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nbr);
	rules->dead_lock = sem_open("/dead_lock", O_CREAT, 0644, 1);
	rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
	rules->fork_lock = sem_open("/fork_lock", O_CREAT, 0644, 1);
	sem_unlink("/forks");
	sem_unlink("/dead_lock");
	sem_unlink("/print");
	sem_unlink("/fork_lock");
	threads = ft_calloc(rules->nbr, sizeof(pthread_t));
	i = 0;
	while(i < rules->nbr)
	{
		rules->time_start = get_time();
		rules->philo[i].id = i;
		rules->philo[i].time_last_eat = get_time();
		rules->philo[i].nbr_eat = 0;
		if (pthread_create(&threads[i], NULL, bigbrother, &rules->philo[i]) == -1)
		{
			printf("thread failed\n");
			exit (EXIT_FAILURE);
			// exit_thread(rules);
		}
		sem_wait(rules->fork_lock);
		rules->id[i] = fork();
		if (rules->id[i] == 0)
		{
			routine(&rules->philo[i]);
			exit(EXIT_SUCCESS);
		}
		sem_post(rules->fork_lock);
		i++;
		usleep(50);
	}
	i = 0;
	while(i < rules->nbr)
		pthread_join(threads[i++], NULL);
}

void solo_life(t_rules *rules)
{
	int id;

	rules->time_start = get_time();
	id = fork();
	if (id == 0)
	{
		printf("[🍴] %d 1 has taken a fork \n", get_time() - rules->time_start);
		usleep(rules->tt_die * 1000);
		printf("[💀] %d 1 died\n", get_time() - rules->time_start);
	}
	else
		waitpid(-1, NULL, 0);

}

void clean(t_rules *rules)
{
	sem_close(rules->fork_lock);
	sem_close(rules->forks);
	sem_close(rules->dead_lock);
	sem_close(rules->print_lock);
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	set_rules(&rules, argc, argv);
	init_sem(&rules);
	if (rules.nbr > 1)
		life(&rules);
	else if (rules.nbr == 1)
		solo_life(&rules);
	clean(&rules);
	return (0);
}

// ./philo dsdas sd adas dasdasdasda
// faire un thread quand un seul philo
