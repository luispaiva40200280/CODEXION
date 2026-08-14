/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 21:00:02 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/14 01:23:33 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

void	release_dongles(t_data *data, t_dongle *left, t_dongle *right)
{
	long long	time;

	time = get_time() - data->start_time;
	left->last_release = time;
	right->last_release = time;
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_unlock(&right->lock);
}

void	request_right_dongle(t_data *data, t_coder *coder, t_dongle *right)
{
	long long	time;
	long long	time_to_wait;

	pthread_mutex_lock(&right->lock);
	time = get_time() - data->start_time;
	time_to_wait = (right->last_release + data->dongle_cooldown) - time;
	if (right->last_release + data->dongle_cooldown > time)
		ft_usleep(time_to_wait, data);
	ft_print_action(coder, TAKEN);
}

void	request_left_dongle(t_data *data, t_coder *coder, t_dongle *left)
{
	long long	time;
	long long	time_to_wait;

	pthread_mutex_lock(&left->lock);
	time = get_time() - data->start_time;
	time_to_wait = (left->last_release + data->dongle_cooldown) - time;
	if (left->last_release + data->dongle_cooldown > time)
		ft_usleep(time_to_wait, data);
	ft_print_action(coder, TAKEN);
}

void	ft_request_dongles(t_data *data, t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		request_left_dongle(data, coder, coder->left_dongle);
		request_right_dongle(data, coder, coder->right_dongle);
	}
	else
	{
		request_right_dongle(data, coder, coder->right_dongle);
		request_left_dongle(data, coder, coder->left_dongle);
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
		printf("%lld %i %s coder is burnout%s\n", time, coder->id, RED, RESET);
		return ;
	}
	pthread_mutex_lock(&coder->data->sim_lock);
	if (coder->data->sim_active)
	{
		pthread_mutex_lock(&data->write_lock);
		if (action == TAKEN)
			printf("%lld %i %scoder has taken a dongle%s\n", time, coder->id, BLUE, RESET);
		if (action == COMPILING)
			printf("%lld %i %scoder is compiling%s\n", time, coder->id, GREEN, RESET);
		if (action == DEBUGING)
			printf("%lld %i %scoder is debuging%s\n", time, coder->id, YELLOW, RESET);
		if (action == REFACTORING)
			printf("%lld %i %scoder is refacturing%s\n", time, coder->id, MAGENTA, RESET);
		pthread_mutex_unlock(&data->write_lock);
	}
	pthread_mutex_unlock(&coder->data->sim_lock);
}
