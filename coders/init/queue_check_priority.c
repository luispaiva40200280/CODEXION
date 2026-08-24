/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_check_priority.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 01:02:42 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/24 03:14:34 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static int	is_neighbor(t_coder *coder, t_coder *neighbor, t_dongle *dongle)
{
	if (coder == neighbor)
		return (0);
	if (neighbor->left_dongle != dongle && neighbor->right_dongle != dongle)
		return (0);
	return (1);
}

int	check_global_priority(t_coder *coder, t_dongle *dongle)
{
	t_coder	*candidate;
	int		i;

	pthread_mutex_lock(&coder->data->queue->queue_lock);
	i = -1;
	while (++i < coder->data->queue->size)
	{
		candidate = coder->data->queue->coders[i];
		if (is_neighbor(coder, candidate, dongle))
		{
			if (compare_priority(candidate, coder, coder->data))
			{
				pthread_mutex_unlock(&coder->data->queue->queue_lock);
				return (0);
			}
		}
	}
	pthread_mutex_unlock(&coder->data->queue->queue_lock);
	return (1);
}
