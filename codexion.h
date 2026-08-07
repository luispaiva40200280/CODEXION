/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 17:58:08 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/04 19:00:22 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define _DEFAULT_SOURCE
# define _XOPEN_SOURCE 500

/*Includes */
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

/* MACROS
 * 1 - ACTIONS FOR coders
 * 2 -scheduler of queue  
 * 3 - Coolors for the therminal
*/
# define	WAITING 5
# define	BURN_OUT 0
# define	TAKEN 1
# define	COMPILING 2
# define	DEBUGING 3
# define 	REFACTORING 4

# define	EDF 2
# define	FIFO 1

#ifndef COOLORS
# define  COOLORS 0
#endif

typedef struct s_dongle t_dongle;

typedef struct s_coder t_coder;

typedef struct s_heap
{
	struct s_coder	**coders;
	int	max_leng;
	int	size;
	int	type;
	struct s_data	*data;
	pthread_mutex_t	queue_lock;
}	t_heap;

typedef struct s_data
{
	int			number_of_coders;
	long long		start_time;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	int			scheduler;
	struct s_dongle		*dongles;
	struct s_coder		*coders;
	struct s_heap    	*queue;
  	int			sim_active;
	pthread_t		og_thread;
	pthread_mutex_t		sim_lock;
	pthread_mutex_t		write_lock;
}		t_data;

typedef struct s_coder
{
	int		id;
	long long	last_compile_start;
	long long	request_time;
	int		max_compiles;
	int		nbr_of_compiles;
	int   		action;
	pthread_t	thread;
	pthread_cond_t	wait;
	struct s_data	*data;
	t_dongle 	*right_dongle;
	t_dongle 	*left_dongle;
}	t_coder;

typedef struct s_dongle
{
	int		id;
	int		is_cooldown;
	int		is_taken;
	long long	time_cooldown;
	pthread_mutex_t lock;
}	t_dongle;

/*Prototipes*/
int	ft_parser(char **av, t_data *rules);
int	init_data_lists(t_data *data);
int	compare_priority(t_coder *coder_a, t_coder *coder_b, t_data *data);
void	ft_heappush(t_data *data, t_coder *coder);
t_coder	*ft_heappop(t_heap *queue);
void	swap_coders(t_heap *queue, int i, int j);
void	heap_shift_down(t_heap *queue, int index);

/*INIT OF DONGLES AND CODERS*/
int	init_data_lists(t_data *data);
int	init_coders(t_data *data);
int	init_dongles(t_data *data);

/*Time functions*/
long long	get_time(void);
void	ft_usleep(long long time, t_data *data);

/*SIMULATION */
void	*coder_routine(void *args);
void		run_sim_coder(t_coder *coder);
void		run_simulation(t_data *data);


/*PRINTING ACTION*/
void	ft_print_action(t_coder *coder, int action);

#endif
