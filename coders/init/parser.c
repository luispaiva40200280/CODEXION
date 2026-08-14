/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 00:08:29 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/14 00:51:13 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

int	check_scheduler(const char *scheduler)
{
	if (strcmp(scheduler, "edf") == 0)
		return (EDF);
	else if (strcmp(scheduler, "fifo") == 0)
		return (FIFO);
	else
		return (-1);
}

static int	ft_check_data(t_data *data)
{
	if (data->number_of_compiles_required <= 0)
		return (1);
	if (data->number_of_coders <= 0)
		return (1);
	if (data->time_to_burnout < 0)
		return (1);
	if (data->dongle_cooldown < 0)
		return (1);
	if (data->time_to_refactor < 0 || data->time_to_debug < 0
		|| data->time_to_compile < 0)
		return (1);
	return (0);
}

int	ft_parser(char **av, t_data *data)
{
	data->number_of_coders = atoi(av[1]);
	data->time_to_burnout = atoi(av[2]);
	data->time_to_compile = atoi(av[3]);
	data->time_to_debug = atoi(av[4]);
	data->time_to_refactor = atoi(av[5]);
	data->number_of_compiles_required = atoi(av[6]);
	data->dongle_cooldown = atoi(av[7]);
	data->scheduler = check_scheduler((const char *)av[8]);
	if (data->scheduler == -1)
		return (1);
	pthread_mutex_init(&data->sim_lock, NULL);
	pthread_mutex_init(&data->write_lock, NULL);
	data->sim_active = 1;
	return (ft_check_data(data));
}
