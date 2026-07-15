/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:02:12 by lpaiva            #+#    #+#             */
/*   Updated: 2026/07/13 00:08:53 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_queue(t_data *data)
{
	data->queue = malloc(sizeof(t_coder *));
	if (!data->queue)
		return (1);
	data->queue->size = 0;
	data->queue->coders = malloc(sizeof(t_coder *) * data->number_of_coders);
	if (!data->queue->coders)
		return (1);
	data->queue->max_leng = data->number_of_coders;
	data->queue->type = data->scheduler;
	return (0);
}

int	compare_priority(t_coder *coder_a, t_coder *coder_b, t_data *data)
{
	int	deadline_a;
	int	deadline_b;

	deadline_a = coder_a->last_compile_start + data->time_to_burnout;
	deadline_b = coder_b->last_compile_start + data->time_to_burnout;
	if (data->scheduler == EDF)
		return (deadline_a < deadline_b);
	return (coder_a->request_time < coder_b->request_time);
}
/*
static void	ft_swap_coders(t_coder *coder_a, t_coder *coder_b)
{
}

void	ft_heappush(t_heap *queue, t_coder *coder)
{
	queue->coders[queue->size] = coder;
	queue->size++;
	
}*/
