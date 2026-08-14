/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 19:16:36 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/13 22:23:44 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

void	wake_threads(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->queue->queue_lock);
	i = -1;
	while (++i < data->number_of_coders)
	{
		pthread_cond_signal(&data->coders[i].wait);
	}
	pthread_mutex_unlock(&data->queue->queue_lock);
}

static int	is_coder_burnout(t_data *data, t_coder *coders)
{
	int			i;
	long long	time;

	time = get_time() - data->start_time;
	i = -1;
	while (++i < data->number_of_coders)
	{
		if (time - coders[i].last_compile_start >= data->time_to_burnout)
		{
			pthread_mutex_lock(&data->write_lock);
			ft_print_action(&coders[i], BURN_OUT);
			return (1);
		}
	}
	return (0);
}

static int	all_coders_finish(t_data *data, t_coder *coders)
{
	int	i;

	i = -1;
	while (++i < data->number_of_coders)
	{
		if (coders[i].nbr_of_compiles < data->number_of_compiles_required)
			return (0);
	}
	return (1);
}

void	monitor(t_data *data)
{
	t_coder	*coders;

	coders = data->coders;
	while (1)
	{
		usleep(1000);
		pthread_mutex_lock(&data->sim_lock);
		if (is_coder_burnout(data, coders) || all_coders_finish(data, coders))
		{
			data->sim_active = 0;
			pthread_mutex_unlock(&data->sim_lock);
			wake_threads(data);
			return ;
		}
		pthread_mutex_unlock(&data->sim_lock);
	}
}
