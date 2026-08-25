/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 15:12:40 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/25 01:33:55 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static int	ft_grab_first_dongle(t_coder *coder, t_dongle *dongle, int is_first)
{
	long long	time;

	time = get_time() - coder->data->start_time;
	while (1)
	{
		pthread_mutex_lock(&dongle->lock);
		time = get_time() - coder->data->start_time;
		if (check_dongle_availeble(coder, dongle, time))
			break ;
		pthread_mutex_lock(&coder->data->sim_lock);
		if (coder->data->sim_active == 0)
		{
			pthread_mutex_unlock(&coder->data->sim_lock);
			pthread_mutex_unlock(&dongle->lock);
			return (1);
		}
		pthread_mutex_unlock(&coder->data->sim_lock);
		pthread_mutex_unlock(&dongle->lock);
		ft_wait_or_grab_dongle(coder, dongle);
	}
	dongle->is_taken = 1;
	if (is_first)
		set_coder_state(coder, TAKEN_SECOND);
	pthread_mutex_unlock(&dongle->lock);
	return (0);
}

static t_dongle	*ft_dicede_dongle(t_coder *coder, int i)
{
	if (i == 1)
	{
		if (coder->id % 2 == 0)
			return (coder->left_dongle);
		else
			return (coder->right_dongle);
	}
	if (coder->id % 2 == 0)
		return (coder->right_dongle);
	else
		return (coder->left_dongle);
}

void	set_coder_state(t_coder *coder, int state)
{
	pthread_mutex_lock(&coder->data->queue->queue_lock);
	coder->state = state;
	pthread_mutex_unlock(&coder->data->queue->queue_lock);
}

static int	ft_grab_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	set_coder_state(coder, TAKEN_FIRST);
	if (ft_grab_first_dongle(coder, first, 1))
	{
		set_coder_state(coder, WAITING);
		return (1);
	}
	set_coder_state(coder, TAKEN_SECOND);
	if (ft_grab_first_dongle(coder, second, 0))
	{
		release_one(first);
		set_coder_state(coder, WAITING);
		return (1);
	}
	set_coder_state(coder, WAITING);
	return (0);
}

int	ft_request_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = ft_dicede_dongle(coder, 1);
	second = ft_dicede_dongle(coder, 2);
	if (first == second)
	{
		request_single_dongle(coder);
		return (1);
	}
	if (ft_grab_dongles(coder, first, second))
		return (1);
	log_action(coder, "has taken a dongle", BLUE);
	log_action(coder, "has taken a dongle", BLUE);
	return (0);
}
