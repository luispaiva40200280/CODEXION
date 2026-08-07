/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:56:47 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/04 20:56:50 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *args)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)args;
	data = coder->data;
	while (1)
	{
	}
	return (NULL);
}


/*		while (coder->nbr_of_compiles < data->number_of_compiles_required)
		{
			pthread_mutex_lock(&data->sim_lock);	
			if (data->sim_active == 0)
			{	
				pthread_mutex_unlock(&data->sim_lock);
				return (NULL);
			}	
			pthread_mutex_unlock(&data->sim_lock);
			pthread_mutex_lock(&data->queue->queue_lock);
			coder->request_time = get_time() - data->start_time;
			ft_heappush(data, coder);
			while (data->queue->coders[0] != coder)
        	    		pthread_cond_wait(&coder->wait, &data->queue->queue_lock);
        		ft_heappop(data->queue);
			pthread_mutex_unlock(&data->queue->queue_lock);
			pthread_mutex_lock(&coder->left_dongle->lock);
			pthread_mutex_lock(&coder->right_dongle->lock);
			printf("%lld %i coder has taken a dongle\n", coder->request_time, coder->id);
			printf("%lld %i coder has taken a dongle\n", coder->request_time, coder->id);
			coder->last_compile_start = get_time() - data->start_time;
			ft_usleep(data->time_to_compile, data);
			printf("%lld %i coder is compiling\n", coder->request_time, coder->id);
			coder->nbr_of_compiles++;
			pthread_mutex_unlock(&coder->left_dongle->lock);
			pthread_mutex_unlock(&coder->right_dongle->lock);	
			wake_threads(data);
	
			coder->request_time = get_time() - data->start_time;
			printf("%lld %i coder is debugging\n",coder->request_time, coder->id);
			ft_usleep(data->time_to_debug, data);
			coder->request_time = get_time() - data->start_time;
			printf("%lld %i coder is refactoring\n",coder->request_time, coder->id);
			ft_usleep(data->time_to_refactor, data);
		}
		if (coder->nbr_of_compiles == data->number_of_compiles_required)
			return (NULL);
	}*/
