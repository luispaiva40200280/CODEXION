/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 20:34:35 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/20 22:30:23 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static void	ft_compile(t_data *data, t_coder *coder)
{
	pthread_mutex_lock(&data->queue->queue_lock);
	coder->request_time = get_time() - data->start_time;
	pthread_mutex_unlock(&data->queue->queue_lock);
	ft_request_dongles(coder);
	pthread_mutex_lock(&data->sim_lock);
	if (data->sim_active == 0)
	{
		pthread_mutex_unlock(&data->sim_lock);
		return ;
	}
	pthread_mutex_unlock(&data->sim_lock);
	ft_print_action(coder, COMPILING);
	ft_usleep(data->time_to_compile, data);
	pthread_mutex_lock(&data->sim_lock);
	coder->last_compile_start = get_time() - data->start_time;
	coder->nbr_of_compiles++;
	pthread_mutex_unlock(&data->sim_lock);
	release_dongles(data, coder->left_dongle, coder->right_dongle);
	wake_threads(data);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)arg;
	data = coder->data;
	while (1)
	{
		pthread_mutex_lock(&data->sim_lock);
		if (data->sim_active == 0
			|| coder->nbr_of_compiles >= data->number_of_compiles_required)
		{
			pthread_mutex_unlock(&data->sim_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&data->sim_lock);
		ft_compile(data, coder);
		ft_print_action(coder, DEBUGING);
		ft_usleep(data->time_to_debug, data);
		ft_print_action(coder, REFACTORING);
		ft_usleep(data->time_to_refactor, data);
	}
}
