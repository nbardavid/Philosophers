/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:51:42 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/06 14:24:27 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>

# define SIZE_MAX (18446744073709551615UL)

typedef struct s_rules
{
	int		nbr;
	int		tt_die;
	int		tt_eat;
	int		tt_sleep;
	int		tt_think;
	int		nbr_eat;
	int		time_start;
	int		died;
	pthread_mutex_t time_lock;
	pthread_mutex_t print_lock;
	pthread_mutex_t died_lock;
	pthread_mutex_t *forks;
	struct s_philo *philo;
}			t_rules;

typedef struct s_philo
{
	int		id;
	int		time_last_eat;
	int		nbr_eat;
	pthread_mutex_t eat_lock;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_rules	*rules;
}			t_philo;

/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- UTILS -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

int		ft_atoi(char *str);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_sleep(int ms, t_philo *philo);
int		get_time(void);
void	free_rules(t_rules *rules);
int		check_dead(t_philo *philo, int *trigger);

/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- INIT -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

void	set_philo(t_rules *rules);
void	set_rules(t_rules *rules, int argc, char **argv);

void	check_args(t_rules *rules, int argc, char **argv);
void	check_argc(int argc);


/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- ROUTINE -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

void	eating(t_philo *philo);
void	*routine(void *philo_ptr);

/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- CHECKER -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

void	*checker(void	*rules_ptr);

#endif
