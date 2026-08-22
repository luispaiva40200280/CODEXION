/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 18:07:05 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/22 18:29:59 by lpaiva           ###   ########.fr       */
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
# include <limits.h>

/*
parser and initialization of all Data
necessary for the coders
*/
int				check_args(int ac, char **av);
int				ft_parser_init(char **av, t_data *data);
int				ft_init_all(t_data *data);

/*
Time functions using gettimeofday an usleep
*/
long long		get_time(void);
void			ft_usleep(long long time, t_data *data);
struct timespec	ft_calc_time_cooldown(t_dongle *dongle,
					long long start_time_sim);

/*
Simulation and routine of the coders
*/

void			*routine(void *arg);
void			monitor(t_data *data);
void			request_right_dongle(t_coder *coder, t_dongle *right);
void			request_left_dongle(t_coder *coder, t_dongle *left);
void			wake_threads(t_data *data);
void			release_dongles(t_data *data, t_dongle *left, t_dongle *right);
void			log_action(t_coder *coder, const char *msg, const char *color);

/*
Heap queue functions
*/
void			ft_heappush(t_data *data, t_coder *coder);
t_coder			*ft_heappop(t_heap *queue);
void			ft_remove_queue(t_heap *queue, t_coder *coder);
int				compare_priority(t_coder *coder_a, t_coder *coder_b,
					t_data *data);
int				check_global_priority(t_coder *coder, t_dongle *dongle);
void			heap_shift_down(t_heap *queue, int index);
void			heap_shift_up(t_heap *queue, int index);
void			swap_coders(t_heap *queue, int i, int j);

#endif