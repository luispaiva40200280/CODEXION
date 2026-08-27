/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 15:12:40 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/27 02:50:41 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

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

static int	ft_grab_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	long long	time;

	time = get_time() - coder->data->start_time;
	if (first->is_taken || time - first->time_to_cooldown < first->last_release)
		return (1);
	if (second->is_taken || time - second->time_to_cooldown < second->last_release)
		return (1);
	first->is_taken = 1;
	second->is_taken = 1;
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
	while (1)
	{
		pthread_mutex_lock(&coder->data->sim_lock);
		if (!coder->data->sim_active)
			return (pthread_mutex_unlock(&coder->data->sim_lock), 1);
		if (check_global_priority(coder, first) || check_global_priority(coder, second))
		{
			pthread_mutex_unlock(&coder->data->sim_lock);
			usleep(500);
			continue ;
		}
		if (ft_grab_dongles(coder, first, second) == 0)
		{
			pthread_mutex_unlock(&coder->data->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&coder->data->sim_lock);
		usleep(500);
	}
	log_action(coder, "has taken a dongle", BLUE);
	log_action(coder, "has taken a dongle", BLUE);
	return (0);
}
