/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:51:42 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/13 15:15:25 by nbardavi         ###   ########.fr       */
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
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

# define SIZE_MAX (18446744073709551615UL)
# define EXIT_DEAD 10

typedef struct s_philo
{
	int		id;
	int		time_last_eat;
	int		nbr_eat;
	struct s_rules	*rules;
}			t_philo;

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
	int		*id;
	int		trigger;
	sem_t	*dead_lock;
	sem_t	*print_lock;
	sem_t	*forks;
	t_philo *philo;
}			t_rules;


/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- UTILS -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

int		ft_atoi(char *str);
void	*ft_calloc(size_t nmemb, size_t size);
int		get_time(void);
void	fd_putnbr(int n, int fd);
void	fd_putstr(int fd, char *str);
void	init_sem(t_rules *rules);

/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- INIT -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

void	set_rules(t_rules *rules, int argc, char **argv);
void	ft_sleep(int ms, t_philo *philo);

/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- ROUTINE -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

void eat(t_philo *philo);

/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- CHECKER -._.-=-._.-=-._.-=-._.-=-._.-=-._*/


#endif
