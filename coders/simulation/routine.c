/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 20:34:35 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/23 03:48:15 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static void	ft_compile(t_data *data, t_coder *coder)
{
	pthread_mutex_lock(&data->queue->queue_lock);
	coder->request_time = get_time() - data->start_time;
	ft_heappush(data, coder);
	pthread_mutex_unlock(&data->queue->queue_lock);
	if (ft_request_dongles(coder))
		return ;
	ft_remove_queue(data->queue, coder);
	pthread_mutex_lock(&data->sim_lock);
	if (data->sim_active == 0)
	{
		pthread_mutex_unlock(&data->sim_lock);
		release_dongles(data, coder->left_dongle, coder->right_dongle);
		return ;
	}
	coder->last_compile_start = get_time() - data->start_time;
	log_action(coder, "is compiling", GREEN);
	pthread_mutex_unlock(&data->sim_lock);
	ft_usleep(data->time_to_compile, data);
	pthread_mutex_lock(&data->sim_lock);
	coder->nbr_of_compiles++;
	pthread_mutex_unlock(&data->sim_lock);
	release_dongles(data, coder->left_dongle, coder->right_dongle);
	wake_threads(data);
}

static void	ft_debug_refac(t_data *data, t_coder *coder)
{
	pthread_mutex_lock(&coder->data->sim_lock);
	log_action(coder, "is debugging", YELLOW);
	pthread_mutex_unlock(&coder->data->sim_lock);
	ft_usleep(data->time_to_debug, data);
	pthread_mutex_lock(&coder->data->sim_lock);
	log_action(coder, "is refacturing", MAGENTA);
	pthread_mutex_unlock(&coder->data->sim_lock);
	ft_usleep(data->time_to_refactor, data);
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
		ft_debug_refac(data, coder);
	}
}
