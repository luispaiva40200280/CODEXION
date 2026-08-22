/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 21:00:02 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/22 18:50:51 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

void	release_dongles(t_data *data, t_dongle *left, t_dongle *right)
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
	wake_threads(data);
}

static int	check_dongle_availeble(t_coder *coder, t_dongle *dongle,
		long long time)
{
	if (dongle->is_taken)
		return (0);
	if (time - dongle->last_release < dongle->time_to_cooldown)
		return (0);
	if (!check_global_priority(coder, dongle))
		return (0);
	return (1);
}

void	request_right_dongle(t_coder *coder, t_dongle *right)
{
	struct timespec	time_to_wait;
	long long		time;

	pthread_mutex_lock(&right->lock);
	time = get_time() - coder->data->start_time;
	while (1)
	{
		time = get_time() - coder->data->start_time;
		if (check_dongle_availeble(coder, right, time))
			break ;
		pthread_mutex_lock(&coder->data->sim_lock);
		if (coder->data->sim_active == 0)
		{
			pthread_mutex_unlock(&right->lock);
			pthread_mutex_unlock(&coder->data->sim_lock);
			return ;
		}
		pthread_mutex_unlock(&coder->data->sim_lock);
		time_to_wait = ft_calc_time_cooldown(right, coder->data->start_time);
		pthread_cond_timedwait(&right->cond, &right->lock, &time_to_wait);
	}
	right->is_taken = 1;
	log_action(coder, "has taken a dongle", BLUE);
	pthread_mutex_unlock(&right->lock);
}

void	request_left_dongle(t_coder *coder, t_dongle *left)
{
	struct timespec	time_to_wait;
	long long		time;

	pthread_mutex_lock(&left->lock);
	time = get_time() - coder->data->start_time;
	while (1)
	{
		time = get_time() - coder->data->start_time;
		if (check_dongle_availeble(coder, left, time))
			break ;
		pthread_mutex_lock(&coder->data->sim_lock);
		if (coder->data->sim_active == 0)
		{
			pthread_mutex_unlock(&left->lock);
			pthread_mutex_unlock(&coder->data->sim_lock);
			return ;
		}
		pthread_mutex_unlock(&coder->data->sim_lock);
		time_to_wait = ft_calc_time_cooldown(left, coder->data->start_time);
		pthread_cond_timedwait(&left->cond, &left->lock, &time_to_wait);
	}
	left->is_taken = 1;
	log_action(coder, "has taken a dongle", BLUE);
	pthread_mutex_unlock(&left->lock);
}
