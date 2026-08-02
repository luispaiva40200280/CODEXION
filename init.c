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

int	init_queue(t_data *data)
{
	data->queue = malloc(sizeof(t_heap));
	if (!data->queue)
		return (1);
	data->queue->size = 0;
	data->queue->coders = malloc(sizeof(t_coder *) * data->number_of_coders);
	if (!data->queue->coders)
		return (1);
	data->queue->max_leng = data->number_of_coders;
	data->queue->type = data->scheduler;
	data->queue->data = data;
	return (0);
}

int init_coders(t_heap *queue)
{
	int	i;
	t_data	*data;

	if (!queue || queue->size == 0 || !queue->coders)
		return (1);
	i = 0;
	data = queue->data;
	while (i < queue->max_leng)
	{
		queue->coders[i]->id = i + 1;
		queue->coders[i]->left_dongle = data->dongles[i];
		queue->coders[i]->left_dongle = NULL; 
		i++;
	}
	return (0);
}
