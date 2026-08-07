/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 18:31:50 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/07 18:31:53 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
		usleep(100);
	}
}

long long	get_time(void)
{
	struct timeval	tvalue;
	long long	time;

	if (gettimeofday(&tvalue, NULL) == -1)
		exit(1);
	time = (tvalue.tv_sec * 1000) + (tvalue.tv_usec / 1000);
	return (time);
}

