/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 18:50:42 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/07 18:50:46 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wake_threads(t_data *data)
{
	int i = -1;

	pthread_mutex_lock(&data->queue->queue_lock);
	while (++i < data->number_of_coders)
	{
		pthread_cond_signal(&data->coders[i].wait);
	}
	pthread_mutex_unlock(&data->queue->queue_lock);
}

int	coder_burnout(t_data *data)
{
	int		i;
	long long	time;

	time = get_time() - data->start_time;
	i = -1;
	while (++i < data->number_of_coders)
	{
		if (time - data->coders[i].last_compile_start 
				>= data->time_to_burnout)
		{
			pthread_mutex_lock(&data->write_lock);
			ft_print_action(&data->coders[i], BURN_OUT);	
//			pthread_mutex_unlock(&data->write_lock);
			return (1);
		}
	}
	return (0);
}

int	all_coder_finish(t_data *data)
{
	int		i;
	i = -1;
	while (++i < data->number_of_coders)
	{
		if (data->coders[i].nbr_of_compiles < data->number_of_compiles_required)
			return (0);
	}
	return (1);
}

int	sim_is_over(t_data *data)
{
	pthread_mutex_lock(&data->sim_lock);
	if (coder_burnout(data) || all_coder_finish(data))
	{
		data->sim_active = 0;
		pthread_mutex_unlock(&data->sim_lock);
		wake_threads(data);
		return (1);
	}
	pthread_mutex_unlock(&data->sim_lock);
	return (0);
}

void		run_simulation(t_data *data)
{

	while (1)
	{
		usleep(1000);
		if (sim_is_over(data))
			break ;

	}
}
