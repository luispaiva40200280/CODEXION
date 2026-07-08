/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 17:58:08 by lpaiva            #+#    #+#             */
/*   Updated: 2026/07/08 00:24:28 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

/*Includes */
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

/*STRUCTERS*/
typedef struct s_dongle t_dongle;

typedef struct s_coder t_coder;

typedef struct t_data
{
	int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	int	scheduler;
}		t_data;

typedef struct s_coder
{
	int	val;
}	t_coder;

typedef struct s_dongle
{
	int	data;
}	t_dongle;
/*Prototipes*/
int	ft_parser(char **av, t_data *rules);

#endif
