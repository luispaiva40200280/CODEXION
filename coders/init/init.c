/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 02:20:59 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/17 22:22:23 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static int	ft_init_queue(t_data *data)
{
	data->queue = malloc(sizeof(t_heap));
	if (!data->queue)
		return (printf("%sError:%s Malloc queue failed", RED, RESET), 1);
	data->queue->coders = malloc(sizeof(t_coder *) * data->number_of_coders);
	if (!data->queue->coders)
		return (printf("%sError:%s Malloc queue failed", RED, RESET), 1);
	memset(data->queue->coders, 0, sizeof(t_coder *) * data->number_of_coders);
	data->queue->max_leng = data->number_of_coders;
	data->queue->size = 0;
	data->queue->type = data->scheduler;
	data->queue->data = data;
	pthread_mutex_init(&data->queue->queue_lock, NULL);
	return (0);
}

static int	ft_init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (printf("%sError:%s Malloc failed", RED, RESET), 1);
	memset(data->coders, 0, sizeof(t_coder) * data->number_of_coders);
	i = -1;
	while (++i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[
			(i + 1) % data->number_of_coders];
		data->coders[i].request_time = get_time();
		data->coders[i].nbr_of_compiles = 0;
		data->coders[i].last_compile_start = 0;
		pthread_cond_init(&data->coders[i].wait, NULL);
		data->coders[i].data = data;
	}
	return (0);
}

static int	ft_init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (printf("%sError:%s Malloc failed", RED, RESET), 1);
	memset(data->dongles, 0, sizeof(t_dongle) * data->number_of_coders);
	i = -1;
	while (++i < data->number_of_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].last_release = -data->dongle_cooldown;
		data->dongles[i].time_to_cooldown = data->dongle_cooldown;
		pthread_mutex_init(&data->dongles[i].lock, NULL);
	}
	return (0);
}

int	ft_init_all(t_data *data)
{
	if (!data)
	{
		printf("%sError:%s Initalization failed", RED, RESET);
		return (1);
	}
	if (ft_init_dongles(data))
	{
		printf("%sError:%s Initalization dongles failed", RED, RESET);
		return (1);
	}
	if (ft_init_coders(data))
	{
		printf("%sError:%s Initalization coders failed", RED, RESET);
		return (1);
	}
	if (ft_init_queue(data))
	{
		printf("%sError:%s Initalization dongles failed", RED, RESET);
		return (1);
	}
	return (0);
}
