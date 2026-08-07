/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 03:51:41 by lpaiva            #+#    #+#             */
/*   Updated: 2026/07/30 03:51:48 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

//void	*coder_routine(void *);

int	init_data_lists(t_data *data)
{
	data->queue = malloc(sizeof(t_heap));
	if (!data->queue)
		return (1);
	data->queue->size = 0;
	data->queue->coders = malloc(sizeof(t_coder *) * data->number_of_coders);
	pthread_mutex_init(&data->queue->queue_lock, NULL);
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->queue->coders || !data->coders || !data->dongles)
		return (1);
	memset(data->queue->coders, 0, sizeof(t_coder *) * data->number_of_coders);
	memset(data->coders, 0, sizeof(t_coder) * data->number_of_coders);
	memset(data->dongles, 0, sizeof(t_dongle) * data->number_of_coders);
	data->queue->max_leng = data->number_of_coders;
	data->queue->type = data->scheduler;
	data->queue->data = data;
	return (0);
}

int	init_coders(t_data *data)
{
	int	i;

	if (!data || !data->coders)
		return (1);
	i = -1;
	while (++i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[
			(i + 1) % data->number_of_coders];
		data->coders[i].request_time = get_time();
		data->coders[i].nbr_of_compiles = 0;
		data->coders[i].action = WAITING;
		data->coders[i].last_compile_start = 0;
		//pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]);
		pthread_cond_init(&data->coders[i].wait, NULL);
		data->coders[i].data = data;
	}
	return (0);
}

int	init_dongles(t_data *data)
{
	int		i;

	i = -1;
	while (++i < data->number_of_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].is_cooldown = 0;
		data->dongles[i].is_taken = 0;
		data->dongles[i].time_cooldown =0;
		pthread_mutex_init(&data->dongles[i].lock, NULL);
	}
	return (0);
}
