/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 20:36:06 by lpaiva            #+#    #+#             */
/*   Updated: 2026/07/06 22:27:44 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
static int	ft_check_errors(t_node	*rules);

int	ft_parser(char **av, t_node *rules)
{
	rules->number_of_coders = atoi(av[1]);
	rules->time_to_burnout = atoi(av[2]);
	rules->time_to_compile = atoi(av[3]);
	rules->time_to_debug = atoi(av[4]);
	rules->time_to_refactor = atoi(av[5]);
	rules->number_of_compiles_required = atoi(av[6]);
	rules->dongle_cooldown = atoi(av[7]);
	if (strcmp("fifo", av[8]) == 0)
		rules->scheduler == 1;
	if (strcmp("edf", av[8]) == 0)
		rules->scheduler == 2;
	else
		return (1);
	return (ft_check_errors(rules));	
}

static int	ft_check_errors(t_node	*rules)
{
	if ()	
	return (0)
}
