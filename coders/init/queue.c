/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 21:25:28 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/19 19:13:52 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

int	compare_priority(t_coder *coder_a, t_coder *coder_b, t_data *data)
{
	long long	deadline_a;
	long long	deadline_b;

	deadline_a = coder_a->last_compile_start + data->time_to_burnout;
	deadline_b = coder_b->last_compile_start + data->time_to_burnout;
	if (data->scheduler == EDF)
	{
		if (deadline_a == deadline_b)
			return (coder_a->id < coder_b->id);
		return (deadline_a < deadline_b);
	}
	if (coder_a->request_time == coder_b->request_time)
		return (coder_a->id < coder_b->id);
	return (coder_a->request_time < coder_b->request_time);
}

void	swap_coders(t_heap *queue, int i, int j)
{
	t_coder	*tmp;

	tmp = queue->coders[i];
	queue->coders[i] = queue->coders[j];
	queue->coders[j] = tmp;
}

void	heap_shift_down(t_heap *queue, int index)
{
	int	left_index;
	int	right_index;
	int	winner_coder;

	left_index = (2 * index) + 1;
	while (left_index < queue->size)
	{
		right_index = (2 * index) + 2;
		if (right_index >= queue->size)
			winner_coder = left_index;
		else if (compare_priority(queue->coders[left_index],
				queue->coders[right_index], queue->data))
			winner_coder = left_index;
		else
			winner_coder = right_index;
		if (compare_priority(queue->coders[winner_coder], queue->coders[index],
				queue->data))
		{
			swap_coders(queue, winner_coder, index);
			index = winner_coder;
			left_index = (2 * index) + 1;
		}
		else
			break ;
	}
}

t_coder	*ft_heappop(t_heap *queue)
{
	t_coder	*head;

	if (queue->size == 0)
		return (NULL);
	head = queue->coders[0];
	queue->coders[0] = queue->coders[queue->size - 1];
	queue->size--;
	heap_shift_down(queue, 0);
	return (head);
}

void	ft_heappush(t_data *data, t_coder *coder)
{
	int		index;
	int		parent;
	t_heap	*queue;

	queue = data->queue;
	queue->coders[queue->size] = coder;
	queue->size++;
	parent = queue->size - 1;
	index = parent;
	while (index > 0)
	{
		index = (parent - 1) / 2;
		if (compare_priority(coder, queue->coders[index], data))
		{
			swap_coders(queue, parent, index);
			parent = index;
		}
		else
			break ;
	}
}
