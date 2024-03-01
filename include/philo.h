#ifndef PHILO_H
# define PHILO_H

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <pthread.h>
#include <string.h>

# define SIZE_MAX (18446744073709551615UL)

typedef struct s_rules
{
	int		nbr;
	int		tt_die;
	int		tt_eat;
	int		tt_sleep;
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
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_rules	*rules;
}			t_philo;


/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- UTILS -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

int	ft_atoi(char *str);
void	*ft_calloc(size_t nmemb, size_t size);

#endif
