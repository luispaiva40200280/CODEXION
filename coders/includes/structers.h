/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structers.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 18:07:15 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/18 00:07:17 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTERS_H
# define STRUCTERS_H

# include <pthread.h>

typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;

typedef struct s_heap
{
	struct s_coder		**coders;
	int					max_leng;
	int					size;
	int					type;
	struct s_data		*data;
	pthread_mutex_t		queue_lock;
}						t_heap;

typedef struct s_data
{
	long long			start_time;
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	int					scheduler;
	int					sim_active;
	struct s_dongle		*dongles;
	struct s_coder		*coders;
	struct s_heap		*queue;
	pthread_mutex_t		sim_lock;
	pthread_mutex_t		write_lock;
}						t_data;

typedef struct s_coder
{
	int					id;
	int					nbr_of_compiles;
	long long			last_compile_start;
	long long			request_time;
	t_dongle			*right_dongle;
	t_dongle			*left_dongle;
	pthread_t			thread;
	pthread_cond_t		wait;
	struct s_data		*data;
}						t_coder;

typedef struct s_dongle
{
	int					id;
	long long			last_release;
	int					time_to_cooldown;
	int					is_taken;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
}						t_dongle;

#endif