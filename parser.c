/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 20:36:06 by lpaiva            #+#    #+#             */
/*   Updated: 2026/07/07 19:23:14 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_check_errors(t_data	*rules);

static long long	ft_atoll_parser(const char *str);

static int	check_scheduler(char *str)
{
	if (strcmp(str, "fifo") == 0)
		return (1);
	else if (strcmp(str, "edf") == 0)
		return (2);
	return (-1);
}

int	ft_parser(char **av, t_data *rules)
{
	rules->number_of_coders = atoi(av[1]);
	rules->time_to_burnout = ft_atoll_parser((const char *)av[2]);
	rules->time_to_compile = ft_atoll_parser((const char *)av[3]);
	rules->time_to_debug =   ft_atoll_parser((const char *)av[4]);
	rules->time_to_refactor =  ft_atoll_parser((const char *)av[5]);
	rules->number_of_compiles_required = atoi(av[6]);
	rules->dongle_cooldown = atoi(av[7]);
	rules->scheduler = check_scheduler(av[8]);
	pthread_mutex_init(&rules->sim_lock, NULL);
	pthread_mutex_init(&rules->write_lock, NULL);
	rules->sim_active = 1;
	return (ft_check_errors(rules));
}

static int	ft_check_errors(t_data	*rules)
{
	if (rules->number_of_coders < 0)
		return (printf("Coders need to be more then 2"), 1);
	else if (rules->time_to_compile < 0)
		return (printf("Time for compilation needs to be possitive"), 1);
	else if (rules->time_to_burnout < 0)
		return (printf("Bournout time needs to be possitve"), 1);
	else if (rules->time_to_debug < 0)
		return (printf("Debug time needs to be possitve"), 1);
	else if (rules->time_to_refactor < 0)
		return (printf("Refactor time needs to be possitve"), 1);
	else if (rules->number_of_compiles_required < 0)
		return (printf("Nbr of compilers needs to be possitve"), 1);
	else if (rules->dongle_cooldown < 0)
		return (printf("Cooldown needs to be possitve"), 1);
	else if (rules->scheduler < 0)
		return (printf("Scheduler needs to be only 'fifo' or  'edf' "), 1);
	return (0);
}

static long long	ft_atoll_parser(const char *str)
{
	long long	nbr;

	nbr = 0;
	while (*str)
	{
		nbr = (nbr * 10) + (*str - '0');
		str++;
	}
	return (nbr);
}
