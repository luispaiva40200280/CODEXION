/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 21:00:02 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/21 01:59:53 by lpaiva           ###   ########.fr       */
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

void	request_right_dongle(t_coder *coder, t_dongle *right)
{
	struct timespec	time_to_wait;
	long long		time;

	pthread_mutex_lock(&right->lock);
	time = get_time() - coder->data->start_time;
	while (1)
	{
		time = get_time() - coder->data->start_time;
		if (!right->is_taken && (time
				- right->last_release >= right->time_to_cooldown)
				&& check_global_priority(coder, right))
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
	ft_print_action(coder, TAKEN);
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
		if (!left->is_taken && (time
				- left->last_release >= left->time_to_cooldown)
				&& check_global_priority(coder, left))
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
	ft_print_action(coder, TAKEN);
	pthread_mutex_unlock(&left->lock);
}

void	ft_request_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
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
	if (coder->id % 2 == 0)
	{
		request_left_dongle(coder, coder->left_dongle);
		request_right_dongle(coder, coder->right_dongle);
	}
	else
	{
		request_right_dongle(coder, coder->right_dongle);
		request_left_dongle(coder, coder->left_dongle);
	}
}

void	ft_print_action(t_coder *coder, int action)
{
	long long	time;
	t_data		*data;

	data = coder->data;
	time = get_time() - data->start_time;
	if (action == BURN_OUT && data->sim_active)
	{
		printf("%lld %i %sburned out%s\n", time, coder->id, RED, RESET);
		return ;
	}
	pthread_mutex_lock(&coder->data->sim_lock);
	if (coder->data->sim_active)
	{
		pthread_mutex_lock(&data->write_lock);
		if (action == TAKEN)
			printf("%lld %i %shas taken a dongle%s\n", time, coder->id, BLUE,
				RESET);
		if (action == COMPILING)
			printf("%lld %i %sis compiling%s\n", time, coder->id, GREEN, RESET);
		if (action == DEBUGING)
			printf("%lld %i %sis debuging%s\n", time, coder->id, YELLOW, RESET);
		if (action == REFACTORING)
			printf("%lld %i %sis refacturing%s\n", time, coder->id, MAGENTA,
				RESET);
		pthread_mutex_unlock(&data->write_lock);
	}
	pthread_mutex_unlock(&coder->data->sim_lock);
}
