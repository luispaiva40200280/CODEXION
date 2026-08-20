/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:29:47 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/20 22:40:49 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

int	check_global_priority(t_coder *coder, t_dongle *dongle)
{
	t_coder	*neighbor;
	int		i;
	int		priority;

	pthread_mutex_lock(&coder->data->queue->queue_lock);
	priority = 1;
	i = -1;
	while (++i < coder->data->queue->size)
	{
		neighbor = coder->data->queue->coders[i];
		if (neighbor != coder && (neighbor->left_dongle->id == dongle->id
				|| neighbor->right_dongle->id == dongle->id))
		{
			if (compare_priority(neighbor, coder, coder->data))
			{
				priority = 0;
				break ;
			}
		}
	}
	pthread_mutex_unlock(&coder->data->queue->queue_lock);
	return (priority);
}

void	heap_shift_up(t_heap *queue, int index)
{
	int		parent_index;
	t_data	*data;

	data = queue->data;
	while (index > 0)
	{
		parent_index = (index - 1) / 2;
		if (compare_priority(queue->coders[parent_index], queue->coders[index], data))
		{
			swap_coders(queue, parent_index, index);
			index = parent_index;
		}
		else
			break ;
		parent_index = (index - 1) / 2;
	}
}

void	ft_remove_queue(t_heap *queue, t_coder *coder)
{
	int	i;
	int	size;

	pthread_mutex_lock(&queue->queue_lock);
	i = -1;
	size = queue->size;
	while (++i < queue->size)
	{
		if (queue->coders[i] == coder)
		{
			queue->coders[i] = queue->coders[size - 1];
			queue->size--;
			if (i == queue->size)
				break ;
			if (i > 0 && compare_priority(queue->coders[i], queue->coders[(i - 1) / 2],
					coder->data))
				heap_shift_up(queue, i);
			else
				heap_shift_down(queue, i);
			break ;
		}
	}
	pthread_mutex_unlock(&queue->queue_lock);
}
