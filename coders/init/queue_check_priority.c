/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_check_priority.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 01:02:42 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/25 01:18:37 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static t_dongle	*get_candidate_target(t_coder *candidate)
{
	if (candidate->state == TAKEN_FIRST || candidate->state == WAITING)
	{
		if (candidate->id % 2 == 0)
			return (candidate->left_dongle);
		else
			return (candidate->right_dongle);
	}
	else if (candidate->state == TAKEN_SECOND)
	{
		if (candidate->id % 2 == 0)
			return (candidate->right_dongle);
		else
			return (candidate->left_dongle);
	}
	return (NULL);
}

static int	is_threat(t_coder *coder, t_coder *candidate, t_dongle *dongle)
{
	if (coder == candidate)
		return (0);
	if (get_candidate_target(candidate) == dongle)
	{
		if (compare_priority(candidate, coder, coder->data))
			return (1);
	}
	return (0);
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
		if (is_threat(coder, candidate, dongle))
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
