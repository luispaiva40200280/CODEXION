/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 19:04:55 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/21 02:50:45 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

void	ft_usleep(long long time, t_data *data)
{
	long long	start;

	start = get_time();
	while (time > (get_time() - start))
	{
		pthread_mutex_lock(&data->sim_lock);
		if (data->sim_active == 0)
		{
			pthread_mutex_unlock(&data->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&data->sim_lock);
		usleep(1000);
	}
}

long long	get_time(void)
{
	struct timeval	tvalue;
	long long		time;

	if (gettimeofday(&tvalue, NULL) == -1)
		exit(1);
	time = (tvalue.tv_sec * 1000) + (tvalue.tv_usec / 1000);
	return (time);
}

struct timespec	ft_calc_time_cooldown(t_dongle *dongle,
		long long start_time_sim)
{
	struct timespec	ts;
	long long		time_ms;

	time_ms = start_time_sim + dongle->last_release + dongle->time_to_cooldown;
	ts.tv_sec = time_ms / 1000;
	ts.tv_nsec = (time_ms % 1000) * 1000000;
	return (ts);
}
