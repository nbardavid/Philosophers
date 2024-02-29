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
	int		*forks;
	struct s_philo *philo;
}			t_rules;

typedef struct s_philo
{
	int		id;
	t_rules	rules;
}			t_philo;


/*_.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._*/
/*_.-=-._.-=-._.-=-._.-=-._.- UTILS -._.-=-._.-=-._.-=-._.-=-._.-=-._*/

int	ft_atoi(char *str);
void	*ft_calloc(size_t nmemb, size_t size);

#endif
