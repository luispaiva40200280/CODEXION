/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_action.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 03:06:33 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/22 18:23:27 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

void	log_action(t_coder *coder, const char *msg, const char *color)
{
	long long	time;
	int			id;

	id = coder->id;
	pthread_mutex_lock(&coder->data->write_lock);
	if (coder->data->sim_active)
	{
		time = get_time() - coder->data->start_time;
		printf("%lld %i%s %s %s\n", time, id, color, msg, RESET);
		pthread_mutex_unlock(&coder->data->write_lock);
	}
	else
		pthread_mutex_unlock(&coder->data->write_lock);
}
