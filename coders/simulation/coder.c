/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 21:00:02 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/25 00:30:53 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

void	release_dongles(t_coder *coder, t_data *data, t_dongle *left,
		t_dongle *right)
{
	long long	time;

	time = get_time() - data->start_time;
	pthread_mutex_lock(&right->lock);
	right->is_taken = 0;
	right->last_release = time;
	pthread_cond_broadcast(&right->cond);
	pthread_mutex_unlock(&right->lock);
	pthread_mutex_lock(&left->lock);
	left->is_taken = 0;
	left->last_release = time;
	pthread_cond_broadcast(&left->cond);
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&data->queue->queue_lock);
	coder->state = WAITING;
	pthread_mutex_unlock(&data->queue->queue_lock);
}

int	check_dongle_availeble(t_coder *coder, t_dongle *dongle, long long time)
{
	if (dongle->is_taken)
		return (0);
	if (time - dongle->last_release < dongle->time_to_cooldown)
		return (0);
	if (!check_global_priority(coder, dongle))
		return (0);
	return (1);
}

void	release_one(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->is_taken = 0;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	request_single_dongle(t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&coder->data->sim_lock);
		if (coder->data->sim_active == 0)
		{
			pthread_mutex_unlock(&coder->data->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&coder->data->sim_lock);
		usleep(500);
	}
	return ;
}

void	ft_wait_or_grab_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	time_to_wait;
	long long		time;

	pthread_mutex_lock(&dongle->lock);
	time = get_time() - coder->data->start_time;
	time_to_wait = ft_calc_time_cooldown(dongle, coder->data->start_time);
	if (dongle->is_taken || time
		- dongle->last_release >= dongle->time_to_cooldown)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	else
	{
		time_to_wait = ft_calc_time_cooldown(dongle, coder->data->start_time);
		pthread_cond_timedwait(&dongle->cond, &dongle->lock, &time_to_wait);
	}
	pthread_mutex_unlock(&dongle->lock);
}
