/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 21:00:02 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/27 02:53:16 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

void	release_dongles(t_data *data, t_dongle *left,
		t_dongle *right)
{
	long long	time;

	time = get_time() - data->start_time;
	pthread_mutex_lock(&data->sim_lock);
	right->is_taken = 0;
	right->last_release = time;
	left->is_taken = 0;
	left->last_release = time;
	pthread_mutex_unlock(&data->sim_lock);
}

int	check_dongle_availeble(t_coder *coder, t_dongle *dongle, long long time)
{
	if (dongle->is_taken)
		return (0);
	if (time - dongle->last_release < dongle->time_to_cooldown)
		return (0);
	if (!check_global_priority(coder, dongle))
		return (0);
	return (1);
}

void	request_single_dongle(t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&coder->data->sim_lock);
		if (coder->data->sim_active == 0)
		{
			pthread_mutex_unlock(&coder->data->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&coder->data->sim_lock);
		usleep(500);
	}
	return ;
}
