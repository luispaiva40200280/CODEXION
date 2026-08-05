/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:56:47 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/04 20:56:50 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time(void)
{
	struct timeval	tvalue;
	long long	time;

	if (gettimeofday(&tvalue, NULL) == -1)
		exit(1);
	time = (tvalue.tv_sec * 1000) + (tvalue.tv_usec / 1000);
	return (time);
}

void	*coder_routine(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	while (1)
	{
		coder->request_time = get_time();
		printf("%lld Coder nbr %i", get_time(), coder->id);
		printf("\n");
		return (NULL);
	}
	return (NULL);
}

