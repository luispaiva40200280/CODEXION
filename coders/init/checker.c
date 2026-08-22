/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 19:58:09 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/22 18:27:12 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static int	check_data(char **av)
{
	int		i;
	char	*data;

	i = 0;
	while (++i < 8)
	{
		data = av[i];
		while (*data)
		{
			if (!(*data >= '0' && *data <= '9'))
			{
				printf("%sError:%s %s has a letters\n", RED, RESET, data);
				return (1);
			}
			data++;
		}
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	if (ac != 9)
	{
		printf("%s Error: Simulation needs to have 8 data:\n %s", RED, RESET);
		printf("%s -> Usage: %s", YELLOW, RESET);
		printf("<number_of_coders> <time_to_burnout> ");
		printf("<time_to_compile> <time_to_debug> <time_to_refactor> ");
		printf("<number_of_compiles_required> <dongle_cooldown> <scheduler>\n");
		return (1);
	}
	return (check_data(av));
}
