/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 18:07:05 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/13 22:35:49 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define _DEFAULT_SOURCE
# define _XOPEN_SOURCE 500

/*Includes */
# include "structers.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

/*
parser and initialization of all Data
necessary for the coders
*/
int			check_args(int ac, char **av);
int			ft_parser(char **av, t_data *data);
int			ft_init_all(t_data *data);

/*
Time functions using gettimeofday an usleep
*/
long long	get_time(void);
void		ft_usleep(long long time, t_data *data);

/*
Simulation and routine of the coders
*/

void		*routine(void *arg);
void		monitor(t_data *data);
void		ft_print_action(t_coder *coders, int action);
void		request_right_dongle(t_data *data, t_coder *coder, t_dongle *right);
void		request_left_dongle(t_data *data, t_coder *coder, t_dongle *left);
void		ft_request_dongles(t_data *data, t_coder *coder);
void		wake_threads(t_data *data);
void		release_dongles(t_data *data, t_dongle *left, t_dongle *right);

/*
Heap queue functions
*/
void		ft_heappush(t_data *data, t_coder *coder);
t_coder		*ft_heappop(t_heap *queue);

#endif