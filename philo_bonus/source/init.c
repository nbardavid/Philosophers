/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:48:17 by nbardavi          #+#    #+#             */
/*   Updated: 2024/03/13 15:01:14 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_abs(int nbr)
{
	if (nbr > 0)
		return (nbr);
	return (-nbr);
}

void	check_argc(int argc)
{
	if (argc < 5)
	{
		write(2, "Too few arguments\n", 18);
		exit(EXIT_FAILURE);
	}
	if (argc > 6)
	{
		write(2, "Too many arguments\n", 19);
		exit(EXIT_FAILURE);
	}
}

void numerical_argument(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			fd_putstr(2, "Only numerical arguments\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void numerical_check(char **argv)
{
	numerical_argument(argv[1]);
	numerical_argument(argv[2]);
	numerical_argument(argv[3]);
	numerical_argument(argv[4]);
}

void	check_args(t_rules *rules, int argc, char **argv)
{
	numerical_check(argv);
	rules->nbr = ft_atoi(argv[1]);
	rules->tt_die = ft_atoi(argv[2]);
	rules->tt_eat = ft_atoi(argv[3]);
	rules->tt_sleep = ft_atoi(argv[4]);
	rules->tt_think = ft_abs(rules->tt_eat - rules->tt_sleep);
	rules->died = 0;
	if (argc == 6)
	{
		numerical_argument(argv[5]);
		rules->nbr_eat = ft_atoi(argv[5]);
	}
	else
		rules->nbr_eat = INT_MAX;
	if (rules->nbr < 0 || rules->tt_sleep < 0 || rules->tt_eat < 0
		|| rules->nbr_eat < 0 || rules->tt_die < 0)
	{
		write(2, "Arguments(s) are negatives values\n", 35);
		exit(EXIT_FAILURE);
	}
	if (rules->nbr > 200)
	{
		write(2, "Too many philosophers\n", 22);
		exit(EXIT_FAILURE);
	}
}

void set_philo(t_rules *rules)
{
	int i;
	
	i = 0;
	rules->philo = ft_calloc(rules->nbr, sizeof(t_philo));
	while(i < rules->nbr)
		rules->philo[i++].rules = rules;
}

void	set_malloc_rules(t_rules *rules)
{
	rules->id = ft_calloc(rules->nbr, sizeof(int));
	if (rules->id == NULL)
		exit(EXIT_FAILURE);
}

void	set_rules(t_rules *rules, int argc, char **argv)
{
	check_argc(argc);
	check_args(rules, argc, argv);
	set_malloc_rules(rules);
	set_philo(rules);
	init_sem(rules);
}

