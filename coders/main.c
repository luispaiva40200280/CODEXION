/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 19:50:58 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/27 02:16:27 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
#include "includes/macros.h"
#include "includes/structers.h"

static void	ft_destroy(t_data *data)
{

	if (!data)
		return ;
	pthread_mutex_destroy(&data->sim_lock);
	pthread_mutex_destroy(&data->write_lock);
}

static void	ft_free_all(t_data *data)
{
	if (!data)
		return ;
	ft_destroy(data);
	if (data->queue)
	{
		free(data->queue->coders);
		free(data->queue);
	}
	if (data->coders)
		free(data->coders);
	if (data->dongles)
		free(data->dongles);
	free(data);
}

static void	ft_start_imulation(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = -1;
	while (++i < data->number_of_coders)
		pthread_create(&data->coders[i].thread, NULL, routine,
			&data->coders[i]);
	monitor(data);
	i = -1;
	while (++i < data->number_of_coders)
		pthread_join(data->coders[i].thread, NULL);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (check_args(ac, av))
		return (1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (printf("%sErrror: %s data allocation failed", RED, RESET), 1);
	memset(data, 0, sizeof(t_data));
	if (ft_parser_init(av, data))
	{
		free(data);
		printf("%sErrror:%s Someting went wrong whith the parser", RED, RESET);
		return (1);
	}
	if (ft_init_all(data))
	{
		ft_free_all(data);
		return (1);
	}
	ft_start_imulation(data);
	return (ft_free_all(data), 0);
}
